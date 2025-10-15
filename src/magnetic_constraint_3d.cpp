#include "magnetic_constraint_3d.h"

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

using namespace godot;

/// A custom physics “magnetic” constraint node.
/// Attach this node to your scene and assign two RigidBody3D nodes.
/// The constraint applies a force between the two bodies according to an inverse-square law.
/// The property "polarity" determines the sign:
///   - polarity == 1: attraction (each body is pulled toward the other)
///   - polarity == -1: repulsion (each body is pushed away from the other)

void MagneticConstraint3D::_bind_methods() {
    // Bind the properties so they appear in the editor:
    ClassDB::bind_method(D_METHOD("set_max_distance", "max_distance"), &MagneticConstraint3D::set_max_distance);
    ClassDB::bind_method(D_METHOD("get_max_distance"), &MagneticConstraint3D::get_max_distance);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "max_distance"), "set_max_distance", "get_max_distance");

    ClassDB::bind_method(D_METHOD("set_magnetic_force", "force"), &MagneticConstraint3D::set_magnetic_force);
    ClassDB::bind_method(D_METHOD("get_magnetic_force"), &MagneticConstraint3D::get_magnetic_force);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "magnetic_force"), "set_magnetic_force", "get_magnetic_force");

    ClassDB::bind_method(D_METHOD("set_polarity", "polarity"), &MagneticConstraint3D::set_polarity);
    ClassDB::bind_method(D_METHOD("get_polarity"), &MagneticConstraint3D::get_polarity);
    ADD_PROPERTY(PropertyInfo(Variant::INT, "polarity"), "set_polarity", "get_polarity");

    // Bind a method so that polarity can be switched dynamically.
    ClassDB::bind_method(D_METHOD("toggle_polarity"), &MagneticConstraint3D::toggle_polarity);
}

MagneticConstraint3D::MagneticConstraint3D() {
    // Default values for the constraint.
}

MagneticConstraint3D::~MagneticConstraint3D() {
    // Nothing to clean up in this simple example.
}

// -- Property setters and getters --
void MagneticConstraint3D::set_max_distance(float p_max_distance) { max_distance = p_max_distance; }
float MagneticConstraint3D::get_max_distance() const { return max_distance; }

void MagneticConstraint3D::set_magnetic_force(float p_force) { magnetic_force = p_force; }
float MagneticConstraint3D::get_magnetic_force() const { return magnetic_force; }

void MagneticConstraint3D::set_polarity(int p_polarity) { polarity = (p_polarity >= 0) ? 1 : -1; }
int MagneticConstraint3D::get_polarity() const { return polarity; }

void MagneticConstraint3D::toggle_polarity() {
    polarity = -polarity;
    UtilityFunctions::print("Magnetic polarity toggled. New polarity: ", String::num_int64(polarity));
}

// Enable input processing so that _input() is called.
    void MagneticConstraint3D::_ready() {
        set_process_input(true);

        // Search the scene tree from the root for the nodes by name.
        Node* found_node_a = get_parent()->find_child("Player", true, false);
        Node* found_node_b = get_parent()->find_child("Enemy", true, false);

        if (found_node_a) {
            // Attempt to cast to RigidBody3D.
            body_a = Object::cast_to<RigidBody3D>(found_node_a);
            if (!body_a) {
                UtilityFunctions::print("Found node 'Player' in the scene tree.");
            }
        } else {
            UtilityFunctions::print("Could not find node 'Player' in the scene tree.");
        }

        if (found_node_b) {
            body_b = Object::cast_to<RigidBody3D>(found_node_b);
            if (!body_b) {
                UtilityFunctions::print("Found node 'Enemy' in the scene tree.");
            }
        } else {
            UtilityFunctions::print("Could not find node 'Enemy' in the scene tree.");
        }
    }

    // Check for input events. When the space bar is pressed, toggle the magnetic polarity.
    void MagneticConstraint3D::_input(const Ref<InputEvent> &p_event) {
        Ref<InputEventKey> key_event = p_event;
        if (key_event.is_valid() && key_event->is_pressed()) {
            // Check if the key pressed is the space bar.
            if (key_event->get_keycode() == KEY_SPACE) {
                toggle_polarity();
            }
        }
    }

// Called every physics frame.
void MagneticConstraint3D::_physics_process(double delta) {
    // Make sure both bodies are set.
    if (!body_a || !body_b)
        return;

    // Get the global positions of both bodies.
    Vector3 pos_a = body_a->get_global_transform().get_origin();
    Vector3 pos_b = body_b->get_global_transform().get_origin();
    Vector3 dir = pos_b - pos_a;
    float distance = dir.length();
    // UtilityFunctions::print("Distance: " + String::num(distance));

    // Avoid division by zero or applying force when bodies are too far apart.
    if (distance <= 0.001f || distance > max_distance)
        return;

    // Clamp the distance to avoid very small force values.
    float clamped_distance = MAX(distance, 1.0f);
    // Compute a force magnitude based on an inverse–square law.
    float force_magnitude = magnetic_force / (clamped_distance * clamped_distance);
    // The force vector is along the direction between the bodies.
    // Multiplying by polarity will flip the direction to achieve repulsion.
    Vector3 force_vector = dir.normalized() * (force_magnitude * polarity);

    // Apply the force: if attractive (polarity==1), body_a is pulled toward body_b,
    // and body_b is pulled toward body_a. For repulsion (polarity==-1), the forces are reversed.
    body_a->apply_central_force(force_vector);
    body_b->apply_central_force(-force_vector);
}