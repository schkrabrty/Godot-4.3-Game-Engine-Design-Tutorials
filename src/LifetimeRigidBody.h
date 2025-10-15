#ifndef LIFETIMERIGIDBODY_H
#define LIFETIMERIGIDBODY_H

#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

class LifetimeRigidBody : public RigidBody3D {
    GDCLASS(LifetimeRigidBody, RigidBody3D);

protected:
    static void _bind_methods();

public:
    LifetimeRigidBody();
    ~LifetimeRigidBody();

    // Starts the lifetime timer. When the timer times out, the node is queued for deletion.
    void start_lifetime(float time);

    // Callback method connected to the timer's "timeout" signal.
    void _on_Timer_timeout();
};

} // namespace godot

#endif // LIFETIMERIGIDBODY_H
