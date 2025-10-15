#include "keyinputmodifiedfps.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

void KeyInputModifiedFPS::_bind_methods() {
    // Binding methods to Godot
    ClassDB::bind_method(D_METHOD("move", "direction"), &KeyInputModifiedFPS::move);
    ClassDB::bind_method(D_METHOD("speedController", "delta"), &KeyInputModifiedFPS::speedController);
    
    // Bind speed property with setter and getter
    ClassDB::bind_method(D_METHOD("get_speed"), &KeyInputModifiedFPS::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &KeyInputModifiedFPS::set_speed);
    
    // Expose 'speed' to the inspector
    // ClassDB::add_property("KeyInput", PropertyInfo(Variant::double, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");

    // Bind our new set_input_actions method
    ClassDB::bind_method(
        D_METHOD(
            "set_input_actions", 
            "forward_action", "backward_action", "left_action", "right_action"
        ),
        &KeyInputModifiedFPS::set_input_actions
    );
}

double KeyInputModifiedFPS::get_speed() const {
    return speed;
}

void KeyInputModifiedFPS::set_speed(const double p_speed) {
    speed = p_speed;
}

KeyInputModifiedFPS::KeyInputModifiedFPS() {
    // Constructor
    speed = 200.0;

    // Optionally, set defaults
    _forward_action = "ui_up";
    _backward_action = "ui_down";
    _left_action = "ui_left";
    _right_action = "ui_right";
}

KeyInputModifiedFPS::~KeyInputModifiedFPS() {
    // Destructor
}

void KeyInputModifiedFPS::set_input_actions(
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

void KeyInputModifiedFPS::_process(double delta) {
    // Call the function to determine the speed of the Node in each second
    speedController(delta);

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

void KeyInputModifiedFPS::move(Vector2 direction) {
    translate(direction);
}

void KeyInputModifiedFPS::speedController(double delta) {
    // Accumulate the time that has passed
    time_accumulated += delta;

    // We want 1 second total for the cycle:
    // - 0.0 to 0.33 seconds => speed = 500 (or for around the first 20 frames, the speed will be 500)
    // - 0.33 to 1.0 seconds => speed = 200 (or for the remaining frames, the speed will be 200)
    // Then repeat
    double cycle_duration = 1.0;              // 1 second total
    double first_phase_duration = 1.0 / 3.0;  // first 1/3 of a second (or 20/60 = 1/3)

    if (time_accumulated <= first_phase_duration) {
        speed = 500;
    } else {
        speed = 200;
    }

    // If we've reached the end of our 1-second cycle, reset.
    // This makes the pattern repeat every second.
    if (time_accumulated >= cycle_duration) {
        time_accumulated = 0.0;
    }
}