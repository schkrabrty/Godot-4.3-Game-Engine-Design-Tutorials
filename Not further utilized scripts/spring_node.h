#ifndef SPRING_NODE_H
#define SPRING_NODE_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/input.hpp> // for the Key enum

namespace godot {

class SpringNode : public Node2D {
    GDCLASS(SpringNode, Node2D);

private:
    // Simulation parameters.
    float mass;           // Mass of the node.
    float k;              // Spring constant.
    float damping;        // Damping coefficient.
    float charge_rate;    // Rate at which extra force is accumulated.
    float rest_length;    // Distance from the anchor to the rest position.
    float max_extension;  // How far from the rest position you can pull (in the “active” direction).
    float max_offset;     // How far the node may oscillate in the opposite direction (clamp).

    // Collision parameters.
    float collision_radius;

    // Input & behavior configuration.
    Key activation_key; // The key that “charges” this node (for example, KEY_SPACE or KEY_SHIFT).
    bool reverse_input;   // false for the left–side node, true for the right–side node.

    // Simulation state.
    Vector2 anchor_position;  // Fixed anchor point (set in _ready()).
    Vector2 velocity;         // Current velocity (only x is used here).
    float external_force;     // Accumulated extra force when the key is held down.

protected:
    static void _bind_methods();

public:
    SpringNode();
    ~SpringNode();

    // Main simulation functions.
    void _physics_process(double delta) override;
    void _ready() override;

    // Getters and setters.
    float get_mass() const;
    void set_mass(float p_mass);

    float get_spring_constant() const;
    void set_spring_constant(float p_k);

    float get_damping() const;
    void set_damping(float p_damping);

    float get_charge_rate() const;
    void set_charge_rate(float p_charge_rate);

    float get_rest_length() const;
    void set_rest_length(float p_rest_length);

    float get_max_extension() const;
    void set_max_extension(float p_max_extension);

    float get_max_offset() const;
    void set_max_offset(float p_max_offset);

    float get_collision_radius() const;
    void set_collision_radius(float p_collision_radius);

    int get_activation_key_index() const;
    void set_activation_key_index(int p_index);

    bool get_reverse_input() const;
    void set_reverse_input(bool p_reverse_input);
};

} // namespace godot

#endif // SPRING_NODE_H