#include "customspringcontroller.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// Constructor
CustomSpringController::CustomSpringController() {
    // Optionally, initialize member variables here.
}

// Destructor
CustomSpringController::~CustomSpringController() {
    // Any required cleanup.
}

// --- Setters and Getters ---

void CustomSpringController::set_body_a(RigidBody2D* p_body) {
    body_a = p_body;
}

RigidBody2D* CustomSpringController::get_body_a() const {
    return body_a;
}

void CustomSpringController::set_body_b(RigidBody2D* p_body) {
    body_b = p_body;
}

RigidBody2D* CustomSpringController::get_body_b() const {
    return body_b;
}

void CustomSpringController::set_magnetic_force(float p_force) {
    magnetic_force = p_force;
}

float CustomSpringController::get_magnetic_force() const {
    return magnetic_force;
}

void CustomSpringController::set_max_distance(float p_distance) {
    max_distance = p_distance;
}

float CustomSpringController::get_max_distance() const {
    return max_distance;
}

void CustomSpringController::set_attract(bool p_attract) {
    attract = p_attract;
}

bool CustomSpringController::is_attracting() const {
    return attract;
}

// --- Physics Process: Apply the constraint force each physics frame ---

void CustomSpringController::_physics_process(double delta) {
    if (body_a == nullptr || body_b == nullptr)
        return;

    // Get global positions of both bodies.
    Vector2 pos_a = body_a->get_global_position();
    Vector2 pos_b = body_b->get_global_position();

    // Compute the direction vector and distance between bodies.
    Vector2 direction = pos_b - pos_a;
    float distance = direction.length();
    UtilityFunctions::print("Distance: " + String::num(distance));

    // Only apply force if the bodies are within the effective distance.
    if (distance > 0.001f && distance < max_distance) {
        // Calculate the force magnitude (inverse–square law).
        float force_magnitude = magnetic_force / (distance * distance);
        // Reverse the force if we are repelling.
        if (!attract)
            force_magnitude = -force_magnitude;
        Vector2 force = direction.normalized() * force_magnitude;

        // Apply forces to both bodies in opposite directions.
        body_a->apply_central_force(force);
        body_b->apply_central_force(-force);
    }
}

// --- Binding the methods for GDExtension ---

void CustomSpringController::_bind_methods() {
    // Bind methods for the physics bodies.
    ClassDB::bind_method(D_METHOD("set_body_a", "body"), &CustomSpringController::set_body_a);
    ClassDB::bind_method(D_METHOD("get_body_a"), &CustomSpringController::get_body_a);
    ClassDB::bind_method(D_METHOD("set_body_b", "body"), &CustomSpringController::set_body_b);
    ClassDB::bind_method(D_METHOD("get_body_b"), &CustomSpringController::get_body_b);

    // Bind methods for the magnetic force.
    ClassDB::bind_method(D_METHOD("set_magnetic_force", "force"), &CustomSpringController::set_magnetic_force);
    ClassDB::bind_method(D_METHOD("get_magnetic_force"), &CustomSpringController::get_magnetic_force);

    // Bind methods for the maximum distance.
    ClassDB::bind_method(D_METHOD("set_max_distance", "distance"), &CustomSpringController::set_max_distance);
    ClassDB::bind_method(D_METHOD("get_max_distance"), &CustomSpringController::get_max_distance);

    // Bind methods for the attraction toggle.
    ClassDB::bind_method(D_METHOD("set_attract", "attract"), &CustomSpringController::set_attract);
    ClassDB::bind_method(D_METHOD("is_attracting"), &CustomSpringController::is_attracting);

    // Expose the properties in the editor.
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "body_a", PROPERTY_HINT_NODE_TYPE, "RigidBody2D"),
                 "set_body_a", "get_body_a");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "body_b", PROPERTY_HINT_NODE_TYPE, "RigidBody2D"),
                 "set_body_b", "get_body_b");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "magnetic_force"),
                 "set_magnetic_force", "get_magnetic_force");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance"),
                 "set_max_distance", "get_max_distance");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "attract"),
                 "set_attract", "is_attracting");
}
