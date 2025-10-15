#include "coinscontroller.h"
#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/animated_sprite2d.hpp>
#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>

using namespace godot;

void CoinsController::_bind_methods() {
    // Bind distance property with setter and getter
    ClassDB::bind_method(D_METHOD("get_distance"), &CoinsController::get_distance);
    ClassDB::bind_method(D_METHOD("set_distance", "p_distance"), &CoinsController::set_distance);
    
    // Expose 'distance' to the inspector
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "distance"), "set_distance", "get_distance");
}

double CoinsController::get_distance() const {
    return distance;
}

void CoinsController::set_distance(const double p_distance) {
    distance = p_distance;
}

CoinsController::CoinsController() {
    // Constructor
    distance = 300; // Default value
}

CoinsController::~CoinsController() {
    // Destructor
}

void CoinsController::_init() {
    // Initialization code
    if (Engine::get_singleton()->is_editor_hint()) {
        // Skip runtime-specific initialization
        return;
    }
}

void CoinsController::_ready() {
    set_process(true); // If you want `_process()` to run, ensure processing is active
}

void CoinsController::_process(double delta) {
    if (Engine::get_singleton()->is_editor_hint()) {
        // Avoid executing runtime logic in the editor
        return;
    }

    Node *parent_node = get_parent(); // This script will be attached to a simple Node which will consists of all the rotating coins as children. 
                                      // This node and the "Player" node will be children of a main node called let's say "Selective Node Processing".
                                      // So, in order to find the "Player" node, we will first find the parent node, and then find its child "Player".
    if (!parent_node) {
        return;
    }

    CharacterBody2D *player = parent_node->get_node<CharacterBody2D>("Player");

    if (!player) {
        return; // Could not find "Player" as a child of the parent. Adjust path as needed.
    }

    Vector2 player_pos = player->get_global_position(); // Get the player's global position for distance checks
    const int child_count = get_child_count(); // Loop over all child coins of this node ("Coins")

    for (int i = 0; i < child_count; i++) {
        Node *child_node = get_child(i);
        Area2D *coin_area = Object::cast_to<Area2D>(child_node); // We expect each child to be an Area2D for the coin

        if (!coin_area) {
            continue; // Skip if it's not actually a coin
        }

        Vector2 coin_pos = coin_area->get_global_position(); // Check the coin's distance from the player
        float dist = coin_pos.distance_to(player_pos); // Distance between player and coin

        AnimatedSprite2D *coin_sprite =
            coin_area->get_node<AnimatedSprite2D>("AnimatedSprite2D"); // Get the AnimatedSprite2D inside the coin. 
                                                                       // The AnimatedSprite2D is a child of an Area2D node.
                                                                       // So, we first access the Area2D, and then we get the AnimatedSprite2D node called "AnimatedSprite2D".

        if (!coin_sprite) {
            continue;
        }

        if (dist <= static_cast<float>(distance)) { // If the distance is less than or equal to the distance set in the inspector, 
                                                   // then play the animation.
            coin_sprite->play("Coin spin");        // Play the animation that you created for the rotating coin earlier.
        } else {
            coin_sprite->stop();                   // Stop the animation if the distance is greater than the distance set in the inspector.
            coin_sprite->set_frame(0);             // Reset the frame to the first frame of the animation.
        }
    }
}

