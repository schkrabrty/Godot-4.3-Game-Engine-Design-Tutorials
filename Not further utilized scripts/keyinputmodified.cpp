#include "keyinputmodified.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

void KeyInputModified::_bind_methods() {
    // Binding methods to Godot
    ClassDB::bind_method(D_METHOD("move", "direction"), &KeyInputModified::move);
    
    // Bind speed property with setter and getter
    ClassDB::bind_method(D_METHOD("get_speed"), &KeyInputModified::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &KeyInputModified::set_speed);
    
    // Expose 'speed' to the inspector
    // ClassDB::add_property("KeyInput", PropertyInfo(Variant::double, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

    // Bind our new set_input_actions method
    ClassDB::bind_method(
        D_METHOD(
            "set_input_actions", 
            "forward_action", "backward_action", "left_action", "right_action"
        ),
        &KeyInputModified::set_input_actions
    );
}

double KeyInputModified::get_speed() const {
    return speed;
}

void KeyInputModified::set_speed(const double p_speed) {
    speed = p_speed;
}

KeyInputModified::KeyInputModified() {
    // Constructor
    speed = 200.0;

    // Optionally, set defaults
    _forward_action = "ui_up";
    _backward_action = "ui_down";
    _left_action = "ui_left";
    _right_action = "ui_right";
}

KeyInputModified::~KeyInputModified() {
    // Destructor
}

void KeyInputModified::set_input_actions(
    const String &p_forward_action,
    const String &p_backward_action,
    const String &p_left_action,
    const String &p_right_action
) {
    _forward_action = p_forward_action;
    _backward_action = p_backward_action;
    _left_action = p_left_action;
    _right_action = p_right_action;
}

void KeyInputModified::_process(double delta) {
    Vector2 direction = Vector2(0, 0);
    Input* input = Input::get_singleton();

    // String x = "Input is = " + String::num_uint64(reinterpret_cast<uint64_t>(input)) + " Direction is = " + "(" + String::num(direction.x) + ", " + String::num(direction.y) + ")";
    // UtilityFunctions::print(x);

    if (input->is_action_pressed(_forward_action)) {
        direction.y -= 1;
    }
    if (input->is_action_pressed(_backward_action)) {
        direction.y += 1;
    }
    if (input->is_action_pressed(_left_action)) {
        direction.x -= 1;
    }
    if (input->is_action_pressed(_right_action)) {
        direction.x += 1;
    }

    move(direction.normalized() * speed * delta);
}

void KeyInputModified::move(Vector2 direction) {
    translate(direction);
}