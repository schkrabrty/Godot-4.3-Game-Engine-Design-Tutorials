#include "wind_2d.h"

#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <cmath>

using namespace godot;

void WindArea2D::_bind_methods() {
    // Register property methods and properties.
    ClassDB::bind_method(D_METHOD("set_wind_direction", "direction"), &WindArea2D::set_wind_direction);
    ClassDB::bind_method(D_METHOD("get_wind_direction"), &WindArea2D::get_wind_direction);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "wind_direction"), "set_wind_direction", "get_wind_direction");

    ClassDB::bind_method(D_METHOD("set_wind_speed", "speed"), &WindArea2D::set_wind_speed);
    ClassDB::bind_method(D_METHOD("get_wind_speed"), &WindArea2D::get_wind_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "wind_speed"), "set_wind_speed", "get_wind_speed");

    ClassDB::bind_method(D_METHOD("set_gust_intensity", "intensity"), &WindArea2D::set_gust_intensity);
    ClassDB::bind_method(D_METHOD("get_gust_intensity"), &WindArea2D::get_gust_intensity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gust_intensity"), "set_gust_intensity", "get_gust_intensity");

    ClassDB::bind_method(D_METHOD("set_gust_frequency", "frequency"), &WindArea2D::set_gust_frequency);
    ClassDB::bind_method(D_METHOD("get_gust_frequency"), &WindArea2D::get_gust_frequency);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gust_frequency"), "set_gust_frequency", "get_gust_frequency");

    ClassDB::bind_method(D_METHOD("set_air_density", "density"), &WindArea2D::set_air_density);
    ClassDB::bind_method(D_METHOD("get_air_density"), &WindArea2D::get_air_density);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "air_density"), "set_air_density", "get_air_density");
}

WindArea2D::WindArea2D() {
    // Default values
    wind_direction = Vector2(1, 0);  // Wind blowing to the right.
    wind_speed = 50.0f;              // Base speed.
    gust_intensity = 0.0f;           // No gusts by default.
    gust_frequency = 0.0f;
    air_density = 1.225f;            // Approximate density of air at sea level.

    // Initialize the accumulated time.
    accumulated_time = 0.0;

    // Enable processing.
    set_process(true);
    set_physics_process(true);
}

WindArea2D::~WindArea2D() {
}

// --- Setters and Getters ---

void WindArea2D::set_wind_direction(const Vector2 &p_direction) {
    // Always keep the direction normalized.
    wind_direction = p_direction.normalized();
}

Vector2 WindArea2D::get_wind_direction() const {
    return wind_direction;
}

void WindArea2D::set_wind_speed(float p_speed) {
    wind_speed = p_speed;
}

float WindArea2D::get_wind_speed() const {
    return wind_speed;
}

void WindArea2D::set_gust_intensity(float p_intensity) {
    gust_intensity = p_intensity;
}

float WindArea2D::get_gust_intensity() const {
    return gust_intensity;
}

void WindArea2D::set_gust_frequency(float p_frequency) {
    gust_frequency = p_frequency;
}

float WindArea2D::get_gust_frequency() const {
    return gust_frequency;
}

void WindArea2D::set_air_density(float p_density) {
    air_density = p_density;
}

float WindArea2D::get_air_density() const {
    return air_density;
}

// --- Helper function to compute a gust multiplier ---
// (This is a basic example. In a real simulation you might use noise functions or more complex models.)
static float compute_gust_factor(float intensity, float frequency, double time) {
    if (frequency <= 0.0f) {
        return 1.0f;
    }
    // Using a sine wave to modulate the wind speed.
    return 1.0f + intensity * sin(frequency * time * Math_PI);
}

void WindArea2D::_physics_process(double delta) {
    // Accumulate the delta time.
    accumulated_time += delta;

    // Compute a gust factor based on the accumulated time.
    float gust_factor = compute_gust_factor(gust_intensity, gust_frequency, accumulated_time);

    // Calculate the current wind vector.
    Vector2 wind_vector = wind_direction * wind_speed * gust_factor;

    // Get all bodies overlapping with this area.
    Array overlapping_bodies = get_overlapping_bodies();
    for (int i = 0; i < overlapping_bodies.size(); i++) {
        Object *obj = overlapping_bodies[i];
        if (obj->is_class("RigidBody2D")) {
            RigidBody2D *body = Object::cast_to<RigidBody2D>(obj);
            if (body) {
                // Get the body's current velocity.
                Vector2 body_velocity = body->get_linear_velocity();

                // Compute the relative velocity (wind relative to the body’s motion).
                Vector2 relative_velocity = wind_vector - body_velocity;

                // Simplified drag force calculation:
                // F_drag = 0.5 * air_density * v² * (Cd * A)
                // Here we assume (Cd * A) is a constant (e.g., 0.1) for demonstration.
                float drag_coefficient = 0.1f;
                float speed = relative_velocity.length();
                Vector2 force_direction = (speed > 0.0f) ? relative_velocity.normalized() : Vector2(0, 0);
                float drag_magnitude = 0.5f * air_density * speed * speed * drag_coefficient;
                Vector2 drag_force = force_direction * drag_magnitude;

                // Optionally, add a small lift force perpendicular to the wind direction.
                float lift_coefficient = 0.05f;
                Vector2 perpendicular = Vector2(-force_direction.y, force_direction.x);
                float lift_magnitude = 0.5f * air_density * speed * speed * lift_coefficient;
                Vector2 lift_force = perpendicular * lift_magnitude;

                // Combine drag and lift to form the total force.
                Vector2 total_force = drag_force + lift_force;

                // Apply the aerodynamic force to the rigid body.
                body->apply_central_force(total_force);
            }
        }
    }
}
