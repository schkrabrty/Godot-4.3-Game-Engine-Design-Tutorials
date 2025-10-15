#ifndef KEYINPUTMODIFIED_H
#define KEYINPUTMODIFIED_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot{
	class KeyInputModified : public Node2D {
	GDCLASS(KeyInputModified, Node2D)

	private:
		double speed;

		// Store the names of the actions to check
		String _forward_action;
		String _backward_action;
		String _left_action;
		String _right_action;

	protected:
		static void _bind_methods();

	public:
		KeyInputModified();
		~KeyInputModified();

		void _process(double delta) override;
		void move(Vector2 direction);

		// Getter and Setter
		double get_speed() const;
		void set_speed(const double p_speed);

		// New method to configure the actions
    	void set_input_actions(
        	const String &p_forward_action,
			const String &p_backward_action,
			const String &p_left_action,
			const String &p_right_action
		);
	};
}

#endif