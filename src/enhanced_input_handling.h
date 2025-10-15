#ifndef ENHANCED_INPUT_HANDLING_H
#define ENHANCED_INPUT_HANDLING_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/character_body2d.hpp>

namespace godot{

	class Enhanced_Input_Handling : public Node2D {
	GDCLASS(Enhanced_Input_Handling, Node2D)

    private:
        CharacterBody2D *player;
        double originalSpeed;
        double newSpeed;
        bool shift_pressed = false;
        int movement_count = 0;

	protected:
		static void _bind_methods();

	public:
		Enhanced_Input_Handling();
		~Enhanced_Input_Handling();

		void _init();
		void _ready() override;  // We’ll set_process_input(true) here
        void _input(const Ref<InputEvent> &event) override;

        // Getter and Setter
		double get_newSpeed() const;
		void set_newSpeed(const double p_speed);
	};
}

#endif