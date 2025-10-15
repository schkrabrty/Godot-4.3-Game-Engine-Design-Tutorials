#ifndef CUSTOMSPRINGCONTROLLER_H
#define CUSTOMSPRINGCONTROLLER_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/rigid_body2d.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot{
	class CustomSpringController : public Node2D {
	GDCLASS(CustomSpringController, Node2D)

	private:
		// References to the two rigid bodies.
        RigidBody2D* body_a;
        RigidBody2D* body_b;

        // Constraint parameters.
        float magnetic_force = 10.0f; // Base strength of the force.
        float max_distance = 300.0f;  // Maximum effective distance.
        bool attract = true;          // true: attraction, false: repulsion.

	protected:
		// Bind the methods and properties to Godot.
        static void _bind_methods();

	public:
		CustomSpringController();
		~CustomSpringController();

		// Setters and getters for the first body.
        void set_body_a(RigidBody2D *p_body);
        RigidBody2D* get_body_a() const;

        // Setters and getters for the second body.
        void set_body_b(RigidBody2D *p_body);
        RigidBody2D* get_body_b() const;

        // Setters and getters for the magnetic force.
        void set_magnetic_force(float p_force);
        float get_magnetic_force() const;

        // Setters and getters for the maximum distance.
        void set_max_distance(float p_distance);
        float get_max_distance() const;

        // Setters and getters for the attraction toggle.
        void set_attract(bool p_attract);
        bool is_attracting() const;

        // Physics processing function.
        void _physics_process(double delta) override;
	};
}

#endif