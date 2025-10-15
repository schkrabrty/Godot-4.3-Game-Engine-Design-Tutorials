#ifndef COLLISION_RESPONSE_CONTROLLER_H
#define COLLISION_RESPONSE_CONTROLLER_H

#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/classes/physics_direct_body_state2d.hpp>
#include <godot_cpp/classes/collision_polygon2d.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>

namespace godot {

class CollisionResponseController : public RigidBody2D {
    GDCLASS(CollisionResponseController, RigidBody2D);

private:
    // The minimum impact speed required to “dent” the body.
    float impact_threshold = 200.0f; 
    // Count the number of collisions (or dents) that have occurred.
    int collision_count = 0;
    // If true, this node represents the player; otherwise, it’s the enemy.
    bool is_player = false;

    // Cooldown timer to prevent continuous texture updates.
    float dent_cooldown_timer = 0.0f;
    // How long to wait (in seconds) before accepting another dent.
    static constexpr float DENT_COOLDOWN_DURATION = 1.0f; // Adjust as needed (1 second)

protected:
    static void _bind_methods();

public:
    CollisionResponseController();
    ~CollisionResponseController();
    
    // This function is called during the physics step and gives access to contact info.
    void _integrate_forces(PhysicsDirectBodyState2D *state) override;

    // This method simulates applying a dent (for example, you might modify a sprite or shader).
    void create_dent();

    // Getters and setters for the threshold so it can be set from the editor.
    void set_impact_threshold(float p_value);
    float get_impact_threshold() const;

    // Setters and getters for is_player.
    void set_is_player(bool p_value);
    bool get_is_player() const;

    // Simulate a dent on the collision polygon.
    // impact_point: the local point (in the CollisionPolygon2D’s coordinate space)
    // dent_strength: how much to move the vertices (in pixels)
    // dent_radius: the area (radius) affected by the dent.
    void simulate_dent(const Vector2 &impact_point, float dent_strength, float dent_radius);
};

} // namespace godot

#endif // COLLISION_RESPONSE_CONTROLLER_H
