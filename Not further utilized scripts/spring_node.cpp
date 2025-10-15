#include "spring_node.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

SpringNode::SpringNode() {
    // Default simulation values.
    mass         = 1.0;
    k            = 10.0;
    damping      = 1.0; // 2.0
    charge_rate  = 200.0; // 50.0
    rest_length  = 200.0;   // How far from the anchor the node is at rest. 200.0
    max_extension = 150.0;  // Maximum additional distance you can pull it away.
    max_offset    = 150.0;  // Maximum movement in the opposite direction.

    // Collision radius.
    collision_radius = 20.0;

    // Default input configuration.
    activation_key = Key::KEY_SPACE; // Initialize to Space.
    reverse_input  = false;           // false means “pull left” (for left node).

    // Initialize simulation state.
    velocity = Vector2(0, 0);
    external_force = 0.0;
}

SpringNode::~SpringNode() {
    // Nothing to clean up in this simple example.
}

void SpringNode::_bind_methods() {
    // Bind getters and setters.
    ClassDB::bind_method(D_METHOD("get_mass"), &SpringNode::get_mass);
    ClassDB::bind_method(D_METHOD("set_mass", "mass"), &SpringNode::set_mass);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "mass"), "set_mass", "get_mass");

    ClassDB::bind_method(D_METHOD("get_spring_constant"), &SpringNode::get_spring_constant);
    ClassDB::bind_method(D_METHOD("set_spring_constant", "k"), &SpringNode::set_spring_constant);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "spring_constant"), "set_spring_constant", "get_spring_constant");

    ClassDB::bind_method(D_METHOD("get_damping"), &SpringNode::get_damping);
    ClassDB::bind_method(D_METHOD("set_damping", "damping"), &SpringNode::set_damping);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "damping"), "set_damping", "get_damping");

    ClassDB::bind_method(D_METHOD("get_charge_rate"), &SpringNode::get_charge_rate);
    ClassDB::bind_method(D_METHOD("set_charge_rate", "charge_rate"), &SpringNode::set_charge_rate);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "charge_rate"), "set_charge_rate", "get_charge_rate");

    ClassDB::bind_method(D_METHOD("get_rest_length"), &SpringNode::get_rest_length);
    ClassDB::bind_method(D_METHOD("set_rest_length", "rest_length"), &SpringNode::set_rest_length);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "rest_length"), "set_rest_length", "get_rest_length");

    ClassDB::bind_method(D_METHOD("get_max_extension"), &SpringNode::get_max_extension);
    ClassDB::bind_method(D_METHOD("set_max_extension", "max_extension"), &SpringNode::set_max_extension);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "max_extension"), "set_max_extension", "get_max_extension");

    ClassDB::bind_method(D_METHOD("get_max_offset"), &SpringNode::get_max_offset);
    ClassDB::bind_method(D_METHOD("set_max_offset", "max_offset"), &SpringNode::set_max_offset);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "max_offset"), "set_max_offset", "get_max_offset");

    ClassDB::bind_method(D_METHOD("get_collision_radius"), &SpringNode::get_collision_radius);
    ClassDB::bind_method(D_METHOD("set_collision_radius", "collision_radius"), &SpringNode::set_collision_radius);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::FLOAT, "collision_radius"), "set_collision_radius", "get_collision_radius");

    ClassDB::bind_method(D_METHOD("get_activation_key_index"), &SpringNode::get_activation_key_index);
    ClassDB::bind_method(D_METHOD("set_activation_key_index", "activation_key_index"), &SpringNode::set_activation_key_index);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::INT, "activation_key_index", PROPERTY_HINT_ENUM, "Space,Shift"),
                        "set_activation_key_index", "get_activation_key_index");

    ClassDB::bind_method(D_METHOD("get_reverse_input"), &SpringNode::get_reverse_input);
    ClassDB::bind_method(D_METHOD("set_reverse_input", "reverse_input"), &SpringNode::set_reverse_input);
    ClassDB::add_property("SpringNode", PropertyInfo(Variant::BOOL, "reverse_input"), "set_reverse_input", "get_reverse_input");
}

float SpringNode::get_mass() const { return mass; }
void SpringNode::set_mass(float p_mass) { mass = p_mass; }

float SpringNode::get_spring_constant() const { return k; }
void SpringNode::set_spring_constant(float p_k) { k = p_k; }

float SpringNode::get_damping() const { return damping; }
void SpringNode::set_damping(float p_damping) { damping = p_damping; }

float SpringNode::get_charge_rate() const { return charge_rate; }
void SpringNode::set_charge_rate(float p_charge_rate) { charge_rate = p_charge_rate; }

float SpringNode::get_rest_length() const { return rest_length; }
void SpringNode::set_rest_length(float p_rest_length) { rest_length = p_rest_length; }

float SpringNode::get_max_extension() const { return max_extension; }
void SpringNode::set_max_extension(float p_max_extension) { max_extension = p_max_extension; }

float SpringNode::get_max_offset() const { return max_offset; }
void SpringNode::set_max_offset(float p_max_offset) { max_offset = p_max_offset; }

float SpringNode::get_collision_radius() const { return collision_radius; }
void SpringNode::set_collision_radius(float p_collision_radius) { collision_radius = p_collision_radius; }

bool SpringNode::get_reverse_input() const { return reverse_input; }
void SpringNode::set_reverse_input(bool p_reverse_input) { reverse_input = p_reverse_input; }

int SpringNode::get_activation_key_index() const {
    // Convert the stored key value to an index.
    // Here 0 corresponds to Space and 1 corresponds to Shift.
    if (activation_key == Key::KEY_SPACE)
        return 0;
    else if (activation_key == Key::KEY_SHIFT)
        return 1;
    return 0; // default to Space if unexpected value
}

void SpringNode::set_activation_key_index(int p_index) {
    // Set activation_key based on the chosen index.
    switch (p_index) {
        case 0:
            activation_key = Key::KEY_SPACE;
            break;
        case 1:
            activation_key = Key::KEY_SHIFT;
            break;
        default:
            activation_key = Key::KEY_SPACE;
            break;
    }
}

void SpringNode::_ready() {
    // Set up the anchor and initial (rest) position based on reverse_input.
    // For the left node (reverse_input == false):
    //   - Anchor is at the left edge.
    //   - Rest position is (anchor + (rest_length, 0)).
    // For the right node (reverse_input == true):
    //   - Anchor is at the right edge.
    //   - Rest position is (anchor + (-rest_length, 0)).
    if (!reverse_input) {
        if (get_viewport()) {
            Rect2 rect = get_viewport()->get_visible_rect();
            anchor_position = Vector2(0, rect.size.y * 0.5);
        } else {
            anchor_position = Vector2(0, 0);
        }
        set_position(anchor_position + Vector2(rest_length, 0));
    } else {
        if (get_viewport()) {
            Rect2 rect = get_viewport()->get_visible_rect();
            anchor_position = Vector2(rect.size.x, rect.size.y * 0.5);
        } else {
            anchor_position = Vector2(0, 0);
        }
        set_position(anchor_position + Vector2(-rest_length, 0));
    }

    // Add the node to a group for collision detection.
    add_to_group("spring_nodes");
}

void SpringNode::_physics_process(double delta) {
    Vector2 pos = get_position();
    float rest_pos_x = (!reverse_input) ? (anchor_position.x + rest_length)
                                         : (anchor_position.x - rest_length);

    // Check whether the activation key is pressed.
    bool key_pressed = Input::get_singleton()->is_key_pressed(activation_key);

    // Update external force and simulate the spring dynamics.
    if (!reverse_input) {
        // LEFT NODE: When the key is pressed, pull left (reduce x).
        if (key_pressed) {
            // Clamp so the node cannot be pulled left past (rest_pos_x - max_extension).
            if (pos.x > rest_pos_x - max_extension) {
                external_force -= charge_rate * delta;
            } else {
                pos.x = rest_pos_x - max_extension;
                velocity.x = 0;
                external_force = 0;
            }
        } else {
            external_force = 0;
        }
        float spring_force = -k * (pos.x - rest_pos_x);
        float damping_force = -damping * velocity.x;
        float total_force = spring_force + damping_force;
        if (key_pressed && pos.x > rest_pos_x - max_extension)
            total_force += external_force;
        float acceleration = total_force / mass;
        velocity.x += acceleration * delta;
        pos.x += velocity.x * delta;

        // Clamp rightward movement (so it doesn’t drift too far to the right).
        float right_limit = rest_pos_x + max_offset;
        if (pos.x > right_limit) {
            pos.x = right_limit;
            velocity.x = 0;
        }
    } else {
        // RIGHT NODE: When the key is pressed, pull right (increase x).
        if (key_pressed) {
            // Clamp so the node cannot be pulled right past (rest_pos_x + max_extension).
            if (pos.x < rest_pos_x + max_extension) {
                external_force += charge_rate * delta;
            } else {
                pos.x = rest_pos_x + max_extension;
                velocity.x = 0;
                external_force = 0;
            }
        } else {
            external_force = 0;
        }
        float spring_force = -k * (pos.x - rest_pos_x);
        float damping_force = -damping * velocity.x;
        float total_force = spring_force + damping_force;
        if (key_pressed && pos.x < rest_pos_x + max_extension)
            total_force += external_force;
        float acceleration = total_force / mass;
        velocity.x += acceleration * delta;
        pos.x += velocity.x * delta;

        // Clamp leftward movement.
        float left_limit = rest_pos_x - max_offset;
        if (pos.x < left_limit) {
            pos.x = left_limit;
            velocity.x = 0;
        }
    }
    set_position(pos);

    // --- Collision Detection ---
    // Check against all other spring nodes in the group "spring_nodes".
    Array nodes = get_tree()->get_nodes_in_group("spring_nodes");
    for (int i = 0; i < nodes.size(); i++) {
        SpringNode *other = Object::cast_to<SpringNode>(nodes[i]);
        if (!other || other == this)
            continue;
        Vector2 other_pos = other->get_position();
        float dist = pos.distance_to(other_pos);
        float min_dist = collision_radius + other->collision_radius;
        if (dist < min_dist && dist > 0) {
            // Calculate collision normal.
            Vector2 normal = (pos - other_pos).normalized();
            // Relative velocity along the normal.
            float rel_vel = (velocity - other->velocity).dot(normal);
            // Only resolve if moving toward each other.
            if (rel_vel < 0) {
                float restitution = 0.8f; // Adjust for bounciness.
                float impulse = -(1 + restitution) * rel_vel / (1 / mass + 1 / other->mass);
                velocity += (impulse / mass) * normal;
                other->velocity -= (impulse / other->mass) * normal;
            }
            // Optionally, separate the nodes if they overlap.
            float overlap = min_dist - dist;
            pos += normal * (overlap * 0.5f);
            other_pos -= normal * (overlap * 0.5f);
            set_position(pos);
            other->set_position(other_pos);
        }
    }
}
