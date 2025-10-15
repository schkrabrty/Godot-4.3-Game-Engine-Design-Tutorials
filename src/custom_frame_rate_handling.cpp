#include "custom_frame_rate_handling.h"
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/classes/viewport.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void CustomFrameRateHandling::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initial_Setup"), &CustomFrameRateHandling::initial_Setup);
    ClassDB::bind_method(D_METHOD("_on_timer_timeout"), &CustomFrameRateHandling::_on_timer_timeout);
}

CustomFrameRateHandling::CustomFrameRateHandling() {
    // Constructor
    parent = nullptr;
}

CustomFrameRateHandling::~CustomFrameRateHandling() {
    // Destructor
}

void CustomFrameRateHandling::_init() {
    // Initialization code
    if (Engine::get_singleton()->is_editor_hint()) {
        // Skip runtime-specific initialization
        return;
    }
}

void CustomFrameRateHandling::_ready() {
    // Call the setup function after ensuring the timer exists
    call_deferred("initial_Setup");
}

void CustomFrameRateHandling::initial_Setup() {
    timer = Object::cast_to<Timer>(get_parent()); // Retrieve the Timer node from the scene
    
    if (!timer || !timer->is_inside_tree()) {
        UtilityFunctions::print("Error: Timer is NOT in the scene tree. Cannot start.");
        return;
    }

    // Get the parent node (which holds movement properties)
    parent = timer->get_parent();

    if (!parent) {
        UtilityFunctions::print("Error: The Timer's parent (Modified_Enemy node) is null.");
        return;
    }

    // Retrieve frame rate from parent
    frame_rate = (parent->has_method("get")) ? float(parent->get("frameRate")) : 60.0f;
    dir = (parent->has_method("get")) ? float(parent->get("direction")) : 1.0f;
    spr_height = (parent->has_method("get")) ? float(parent->get("sprite_height")) : 80.0f;
    speed = (parent->has_method("get")) ? float(parent->get("speedMultiplier")) : 1.0f;

    // Ensure the frame rate is valid
    frame_rate = (frame_rate > 0.0f) ? frame_rate : 60.0f;

    // Configure the Timer
    timer->set_wait_time(1.0f / frame_rate);
    timer->set_one_shot(false);
    timer->start();

    // Connect the timeout signal to the handler
    Error err = timer->connect("timeout", callable_mp(this, &CustomFrameRateHandling::_on_timer_timeout));
    if (err != OK) {
        UtilityFunctions::print("Failed to connect timer timeout signal.");
    } else {
        UtilityFunctions::print("Timer started successfully with frame rate: " + String::num(frame_rate));
    }
}

void CustomFrameRateHandling::_on_timer_timeout() {
    if (Engine::get_singleton()->is_editor_hint()) {
        // Skip runtime-specific initialization
        return;
    }
    
    Area2D *area = Object::cast_to<Area2D>(parent); // Cast parent to Area2D for access to position

    if (!area) {
        UtilityFunctions::print("Parent node is not an Area2D.");
        return;
    }

    Vector2 pos = area->get_position(); // Get the current position of the Area2D node
    pos.y += speed * dir; // Update the y-position based on speed, direction
    Viewport *viewport = get_viewport(); // Retrieve the viewport to get the visible screen size
    
    if (viewport == nullptr) {
        UtilityFunctions::print("Viewport is null.");
        return;
    }

    Rect2 visible_rect = viewport->get_visible_rect();
    Vector2 screen_size = visible_rect.size;

    // Check if the updated position is beyond the top boundary
    if (pos.y < 0.0f) {
        pos.y = 0.0f;         // Clamp to top
        dir = 1.0f;           // Reverse direction to downward
    }
    // Check if the updated position is beyond the bottom boundary
    else if (pos.y > (screen_size.y - spr_height)) {
        pos.y = screen_size.y - spr_height; // Clamp to bottom
        dir = -1.0f;                         // Reverse direction to upward
    }

    // Apply the updated position back to the Area2D node
    area->set_position(pos);
}

