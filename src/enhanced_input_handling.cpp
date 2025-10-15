#include "enhanced_input_handling.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/variant/string.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void Enhanced_Input_Handling::_bind_methods() {
    // Bind newSpeed property with setter and getter
    ClassDB::bind_method(D_METHOD("get_newSpeed"), &Enhanced_Input_Handling::get_newSpeed);
    ClassDB::bind_method(D_METHOD("set_newSpeed", "p_speed"), &Enhanced_Input_Handling::set_newSpeed);
    
    // Expose 'newSpeed' to the inspector
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "newSpeed"), "set_newSpeed", "get_newSpeed");
}

Enhanced_Input_Handling::Enhanced_Input_Handling() {
    // Constructor
    player = nullptr;
    newSpeed = 1000.0; // Default value
}

Enhanced_Input_Handling::~Enhanced_Input_Handling() {
    // Destructor
}

double Enhanced_Input_Handling::get_newSpeed() const {
    return newSpeed;
}

void Enhanced_Input_Handling::set_newSpeed(const double p_speed) {
    newSpeed = p_speed;
}

void Enhanced_Input_Handling::_init() {
    // Initialization code
    if (Engine::get_singleton()->is_editor_hint()) {
        // Skip runtime-specific initialization
        return;
    }
}

void Enhanced_Input_Handling::_ready() {
    set_process_input(true); // Ensure we receive _input() calls
    Node *parent_node = get_parent(); // This script will be attached to a simple Node which will be a child of the "Player" CharacterBody2D node. 
                                      // So, in order to find the "Player" node, we will first find the parent node.

    if (!parent_node) {
        return;
    }

    player = Object::cast_to<CharacterBody2D>(parent_node); // Just check if your "Player" node is definitely a CharacterBody2D node, for safety.

    if (!player) {
        return; // Could not find "Player" as a child of the parent. Adjust path as needed.
    }

    originalSpeed = player->get("speed"); // Get the `speed` variable from the character_body_2d.gd GDScript

    // Initialize the other variables
    shift_pressed = false;
    movement_count = 0;
}

void Enhanced_Input_Handling::_input(const Ref<InputEvent> &event) {    
    if (Engine::get_singleton()->is_editor_hint()) {
        // Skip runtime-specific initialization
        return;
    }
    
    Ref<InputEventKey> key_event = event;

    if (key_event.is_valid() && !key_event->is_echo()) { // Check if this is a KeyEvent and not an echo. Echoes are key repeats.
        bool pressed = key_event->is_pressed(); // `is_pressed() == true` means key just pressed, and false means key just released.
        int key_code = key_event->get_keycode(); // Get the key code of the pressed key.

        // // Debug: print out what key we are dealing with.
        // UtilityFunctions::print("Key code: ", key_code, 
        //                     " pressed=", pressed ? "true" : "false");

        // 1) Check SHIFT
        if (key_code == Key::KEY_SHIFT) {
            shift_pressed = pressed; // True on press, false on release
        }

        // 2) Check movement keys
        if (key_code == Key::KEY_UP || key_code == Key::KEY_DOWN ||
            key_code == Key::KEY_LEFT || key_code == Key::KEY_RIGHT || 
            key_code == Key::KEY_W || key_code == Key::KEY_S || 
            key_code == Key::KEY_A || key_code == Key::KEY_D) {

            if (pressed) {
                movement_count += 1; // A movement key was just pressed
            } else {
                movement_count -= 1; // A movement key was just released

                if (movement_count < 0) {
                    movement_count = 0; // Safety check
                }
            }
        }

        // 3) Update the player's speed.
        // If SHIFT is held AND at least one movement key is down, set sprint speed.
        if (shift_pressed && movement_count > 0) {
            player->set("speed", newSpeed);
        } else {
            player->set("speed", originalSpeed);
        }
    }
}