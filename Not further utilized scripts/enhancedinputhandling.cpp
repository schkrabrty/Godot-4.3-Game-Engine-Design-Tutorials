#include "keyinputmodified.h"
#include "enhancedinputhandling.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

void EnhancedInputHandling::_bind_methods() {
    // Binding methods to Godot
    ClassDB::bind_method(D_METHOD("_on_arrow_pressed", "value", "key_event"), &EnhancedInputHandling::_on_arrow_pressed);
}

EnhancedInputHandling::EnhancedInputHandling() {
    key_input = nullptr;
}

EnhancedInputHandling::~EnhancedInputHandling() {
}

void EnhancedInputHandling::_ready() {
    // Ensure we receive _input() calls
    set_process_input(true);

    // The parent node is presumably KeyInputModified
    Node *parent_node = get_parent();
    key_input = cast_to<KeyInputModified>(parent_node);

    // You could check if (key_input) was null here, just for safety:
    if (!key_input) {
        UtilityFunctions::print("Parent is not a KeyInputModified node!");
    }
}

void EnhancedInputHandling::_input(const Ref<InputEvent> &event) {
    // If we don't have a valid KeyInputModified parent, do nothing
    if (!key_input) {
        return;
    }
    
    // Check if this is a KeyEvent
    Ref<InputEventKey> key_event = event;
    if (key_event.is_valid() && key_event->is_pressed()) {
        // Check if the pressed key is one of the arrow keys
        switch (key_event->get_keycode()) {
            case Key::KEY_UP: {
                _on_arrow_pressed("Up Arrow", key_event);
                break;
            }
            case Key::KEY_DOWN: {
                _on_arrow_pressed("Down Arrow", key_event);
                break;
            }
            case Key::KEY_LEFT: {
                _on_arrow_pressed("Left Arrow", key_event);
                break;
            }
            case Key::KEY_RIGHT: {
                _on_arrow_pressed("Right Arrow", key_event);
                break;
            }
            default:
                break;
        }
    }
}

void EnhancedInputHandling::_on_arrow_pressed(String value, const Ref<InputEventKey> &key_event) {
    bool is_shift_pressed = Input::get_singleton()->is_key_pressed(Key::KEY_SHIFT);

    if (is_shift_pressed) {
        // SHIFT + Arrow
        // UtilityFunctions::print(String("Shift + ") + value + " detected!");
        key_input->set_speed(1000.0);
    } else {
        // Just Arrow
        // UtilityFunctions::print(value + " detected!");
        key_input->set_speed(200.0);
    }
}