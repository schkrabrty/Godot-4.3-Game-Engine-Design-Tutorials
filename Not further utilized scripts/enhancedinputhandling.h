#ifndef ENHANCEDINPUTHANDLING_H
#define ENHANCEDINPUTHANDLING_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

namespace godot{
    // Forward declare KeyInputModified to avoid circular include
    class KeyInputModified;

	class EnhancedInputHandling : public Node2D {
	GDCLASS(EnhancedInputHandling, Node2D)

	private:
		KeyInputModified *key_input;

	protected:
		static void _bind_methods();

	public:
		EnhancedInputHandling();
		~EnhancedInputHandling();

		void _ready() override;  // We’ll set_process_input(true) here
        void _input(const Ref<InputEvent> &event) override;
        void _on_arrow_pressed(String value, const Ref<InputEventKey> &key_event);
	};
}

#endif