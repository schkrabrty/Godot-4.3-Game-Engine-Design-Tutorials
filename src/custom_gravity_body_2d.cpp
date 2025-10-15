#include "custom_gravity_body_2d.h"

#include <godot_cpp/classes/physics_direct_body_state2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>

using namespace godot;

CustomGravityBody2D::CustomGravityBody2D() {
}

CustomGravityBody2D::~CustomGravityBody2D() {
}

void CustomGravityBody2D::set_gravity_strength(float p_strength) {
    gravity_strength = p_strength;
}

float CustomGravityBody2D::get_gravity_strength() const {
    return gravity_strength;
}

void CustomGravityBody2D::set_gravity_mode(int p_mode) {
    gravity_mode = p_mode;
}

int CustomGravityBody2D::get_gravity_mode() const {
    return gravity_mode;
}

void CustomGravityBody2D::set_gravity_center(const Vector2 &p_center) {
    gravity_center = p_center;
}

Vector2 CustomGravityBody2D::get_gravity_center() const {
    return gravity_center;
}

void CustomGravityBody2D::_integrate_forces(PhysicsDirectBodyState2D *state) {
    // Get the current position of the body.
    Vector2 pos = state->get_transform().get_origin();
    Vector2 gravity_force;

    if (gravity_mode == GRAVITY_LOCAL) {
        // Local gravity: Apply a constant force downward.
        gravity_force = Vector2(0, gravity_strength);
    } else if (gravity_mode == GRAVITY_RADIAL) {
        // Radial gravity: Pull toward the defined gravity center.
        Vector2 dir = gravity_center - pos;
        if (dir.length() > 0.001f) {
            gravity_force = dir.normalized() * gravity_strength;
        } else {
            gravity_force = Vector2();
        }
    } else if (gravity_mode == GRAVITY_POINT) {
        // Point gravity: Apply an inverse‑square law force.
        Vector2 diff = pos - gravity_center;
        float distance_sq = diff.length_squared();
        if (distance_sq > 0.001f) {
            // The negative sign ensures the force pulls toward the center.
            gravity_force = -gravity_strength * diff / distance_sq;
        } else {
            gravity_force = Vector2();
        }
    }

    // Apply the computed gravity force to the body.
    state->apply_central_impulse(gravity_force);
}

void CustomGravityBody2D::_bind_methods() {
    // Bind the gravity_strength property.
    ClassDB::bind_method(D_METHOD("set_gravity_strength", "strength"), &CustomGravityBody2D::set_gravity_strength);
    ClassDB::bind_method(D_METHOD("get_gravity_strength"), &CustomGravityBody2D::get_gravity_strength);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "gravity_strength"), "set_gravity_strength", "get_gravity_strength");

    // Bind the gravity_mode property (with an enum hint).
    ClassDB::bind_method(D_METHOD("set_gravity_mode", "mode"), &CustomGravityBody2D::set_gravity_mode);
    ClassDB::bind_method(D_METHOD("get_gravity_mode"), &CustomGravityBody2D::get_gravity_mode);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "gravity_mode", PROPERTY_HINT_ENUM, "Local,Radial,Point"), "set_gravity_mode", "get_gravity_mode");

    // Bind the gravity_center property.
    ClassDB::bind_method(D_METHOD("set_gravity_center", "center"), &CustomGravityBody2D::set_gravity_center);
    ClassDB::bind_method(D_METHOD("get_gravity_center"), &CustomGravityBody2D::get_gravity_center);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "gravity_center"), "set_gravity_center", "get_gravity_center");
}
