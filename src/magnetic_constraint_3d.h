#ifndef MAGNETIC_CONSTRAINT_3D_H
#define MAGNETIC_CONSTRAINT_3D_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

namespace godot {

class MagneticConstraint3D : public Node3D {
    GDCLASS(MagneticConstraint3D, Node3D);

private:
    // Pointers to the two rigid bodies the constraint will work on.
    RigidBody3D* body_a = nullptr;
    RigidBody3D* body_b = nullptr;

    // Maximum distance at which the magnetic force is applied.
    float max_distance = 5.0f;
    // A scaling factor for the magnetic force.
    float magnetic_force = 100.0f; // I found this value to work well in my tests.
    // Polarity: 1 means attractive, -1 means repulsive.
    int polarity = 1;

protected:
    static void _bind_methods();

public:
    MagneticConstraint3D();
    ~MagneticConstraint3D();

    // Main simulation functions.
    void _ready() override;
    void _physics_process(double delta) override;
    void _input(const Ref<InputEvent> &event) override;

    // Setters and getters for the magnetic force.
    void set_magnetic_force(float p_force);
    float get_magnetic_force() const;

    // Setters and getters for the maximum distance.
    void set_max_distance(float p_distance);
    float get_max_distance() const;

    // Setters and getters for the polarity detection.
    void set_polarity(int p_polarity);
    int get_polarity() const;

    void toggle_polarity();
};

} // namespace godot

#endif // MAGNETIC_CONSTRAINT_3D_H