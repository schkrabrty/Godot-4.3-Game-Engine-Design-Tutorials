#include "keyinput.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/input.hpp>

using namespace godot;

void KeyInput::_bind_methods() {
    // Binding methods to Godot
    ClassDB::bind_method(D_METHOD("move", "direction"), &KeyInput::move);
    
    // Bind speed property with setter and getter
    ClassDB::bind_method(D_METHOD("get_speed"), &KeyInput::get_speed);
    ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &KeyInput::set_speed);
    
    // Expose 'speed' to the inspector
    // ClassDB::add_property("KeyInput", PropertyInfo(Variant::double, "speed"), "set_speed", "get_speed");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

double KeyInput::get_speed() const {
    return speed;
}

void KeyInput::set_speed(const double p_speed) {
    speed = p_speed;
}

KeyInput::KeyInput() {
    // Constructor
    speed = 200.0;
}

KeyInput::~KeyInput() {
    // Destructor
}

void KeyInput::_process(double delta) {
    Vector2 direction = Vector2(0, 0);
    Input* input = Input::get_singleton();

    // String x = "Input is = " + String::num_uint64(reinterpret_cast<uint64_t>(input)) + " Direction is = " + "(" + String::num(direction.x) + ", " + String::num(direction.y) + ")";
    // UtilityFunctions::print(x);

    if (input->is_action_pressed("ui_up")) {
        direction.y -= 1;
    }
    if (input->is_action_pressed("ui_down")) {
        direction.y += 1;
    }
    if (input->is_action_pressed("ui_left")) {
        direction.x -= 1;
    }
    if (input->is_action_pressed("ui_right")) {
        direction.x += 1;
    }

    move(direction.normalized() * speed * delta);
}

void KeyInput::move(Vector2 direction) {
    translate(direction);
}