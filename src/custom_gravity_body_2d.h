#ifndef CUSTOM_GRAVITY_BODY_2D_H
#define CUSTOM_GRAVITY_BODY_2D_H

#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/classes/physics_direct_body_state2d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class CustomGravityBody2D : public RigidBody2D {
    GDCLASS(CustomGravityBody2D, RigidBody2D);

public:
    enum GravityMode {
        GRAVITY_LOCAL = 0,
        GRAVITY_RADIAL = 1,
        GRAVITY_POINT = 2,
    };

protected:
    static void _bind_methods();

private:
    // The gravity strength (magnitude). For 2D games using pixel units, you might use a larger value.
    float gravity_strength = 98.0f; 
    // Gravity mode: 0 = Local, 1 = Radial, 2 = Point.
    int gravity_mode = GRAVITY_LOCAL;
    // The center of gravity (used for radial or point gravity).
    Vector2 gravity_center = Vector2(550, 300);

public:
    CustomGravityBody2D();
    ~CustomGravityBody2D();

    // Setters and getters.
    void set_gravity_strength(float p_strength);
    float get_gravity_strength() const;

    void set_gravity_mode(int p_mode);
    int get_gravity_mode() const;

    void set_gravity_center(const Vector2 &p_center);
    Vector2 get_gravity_center() const;

    // Override the physics integration callback.
    virtual void _integrate_forces(PhysicsDirectBodyState2D *state) override;
};

} // namespace godot

#endif // CUSTOM_GRAVITY_BODY_2D_H
