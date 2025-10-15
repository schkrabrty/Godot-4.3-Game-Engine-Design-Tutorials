#include "parentnode.h"
#include "keyinputmodified.h" // So we can cast to KeyInputModified
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/string.hpp>

using namespace godot;

void ParentNode::_bind_methods() {
    // Binding methods to Godot
    ClassDB::bind_method(D_METHOD("SelectiveChildManipulator"), &ParentNode::SelectiveChildManipulator);
}

ParentNode::ParentNode() {
}

ParentNode::~ParentNode() {
}

void ParentNode::_ready() {
    SelectiveChildManipulator();
}

void ParentNode::SelectiveChildManipulator() {
    // Suppose the scene tree is like:
    // ParentNode
    // ├── KeyInputModified1
    // └── KeyInputModified2

    int child_count = get_child_count();

    for (int i = 0; i < child_count; i++) {
        Node *child = get_child(i);

        if (child) {
            // Check if the child is a KeyInputModified node
            KeyInputModified *key_input = cast_to<KeyInputModified>(child);
            
            if (key_input) {
                // Store the child node reference
                if (String(key_input->get_name()) == "KeyInputModified1") {
                    // This child will use WASD. Let's assume we have
                    // custom input actions: "move_up_wasd", "move_down_wasd",
                    // "move_left_wasd", "move_right_wasd" in InputMap.
                    key_input->set_input_actions("move_up_wasd", "move_down_wasd",
                                            "move_left_wasd", "move_right_wasd");
                } else if (String(key_input->get_name()) == "KeyInputModified2") {
                    // This child will use arrow key actions. We'll just set it
                    // to the default "move_up_arrow", "move_down_arrow", "move_left_arrow", "move_right_arrow"
                    // or any other custom arrow-based actions.
                    key_input->set_input_actions("move_up_arrow", "move_down_arrow",
                                            "move_left_arrow", "move_right_arrow");
                }
            }
        }
    }
}