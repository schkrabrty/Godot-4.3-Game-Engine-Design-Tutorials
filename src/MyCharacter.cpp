#include "MyCharacter.h"

using namespace godot;

// Helper function similar to GDScript's move_toward().
static float move_toward(float value, float target, float delta) {
    if (value < target) {
        value = (value + delta < target) ? value + delta : target;
    } else if (value > target) {
        value = (value - delta > target) ? value - delta : target;
    }
    return value;
}

MyCharacter::MyCharacter() {
}

MyCharacter::~MyCharacter() {
}

void MyCharacter::set_sensitivity(float s) {
    sensitivity = s;
}

float MyCharacter::get_sensitivity() const {
    return sensitivity;
}

void MyCharacter::_bind_methods() {    
    // Bind getter and setter for the sensitivity property.
    ClassDB::bind_method(D_METHOD("set_sensitivity", "sensitivity"), &MyCharacter::set_sensitivity);
    ClassDB::bind_method(D_METHOD("get_sensitivity"), &MyCharacter::get_sensitivity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "sensitivity"), "set_sensitivity", "get_sensitivity");
}

void MyCharacter::_enter_tree() {
    // Set multiplayer authority to the local peer's unique ID.
    set_multiplayer_authority(get_name().to_int());
}

void MyCharacter::_ready() {
    // Cache pivot and camera nodes using NodePath.
    pivot = get_node<Node3D>(NodePath("CameraOrigin"));
    cam = get_node<Camera3D>(NodePath("CameraOrigin/SpringArm3D/Camera3D"));
    
    if (cam) {
        // Set camera current state based on whether this instance is authoritative.
        cam->set_current(is_multiplayer_authority());
    }
}

void MyCharacter::_input(const Ref<InputEvent> &event) {
    // Process mouse motion events.
    Ref<InputEventMouseMotion> mouse_motion = event;
    if (mouse_motion.is_valid()) {
        Vector2 rel = mouse_motion->get_relative();
        // Rotate the character around Y based on horizontal mouse movement.
        rotate_y(Math::deg_to_rad(-rel.x * sensitivity));
        // Rotate the pivot around X based on vertical mouse movement.
        if (pivot) {
            pivot->rotate_x(Math::deg_to_rad(-rel.y * sensitivity));
            // Clamp the pivot's X rotation between -90° and 45°.
            Vector3 pivot_rot = pivot->get_rotation();
            pivot_rot.x = Math::clamp(pivot_rot.x, Math::deg_to_rad(-90.0f), Math::deg_to_rad(45.0f));
            pivot->set_rotation(pivot_rot);
        }
    }
}

void MyCharacter::_physics_process(double delta) {
    // Process input and movement only if this instance is authoritative.
    if (is_multiplayer_authority()) {
        // Instead of using your custom velocity variable directly:
        Vector3 new_velocity = get_velocity();

        // Apply gravity if not on the floor.
        if (!is_on_floor()) {
            new_velocity += get_gravity() * static_cast<float>(delta);
        }
        
        // Handle jump input.
        if (Input::get_singleton()->is_action_just_pressed("ui_accept") && is_on_floor()) {
            new_velocity.y = JUMP_VELOCITY;
        }
        
        // Handle quit action.
        if (Input::get_singleton()->is_action_just_pressed("quit")) {
            Node *parent = get_parent();
            if (parent) {
                parent->call("exit_game", get_name().to_int());
            }
            get_tree()->quit();
        }
        
        // Process movement input.
        Vector2 input_dir = Input::get_singleton()->get_vector("move_left_wasd", "move_right_wasd", "move_up_wasd", "move_down_wasd");
        Vector3 direction = get_transform().basis.xform(Vector3(input_dir.x, 0, input_dir.y)).normalized();
        if (direction.length() > 0.0f) {
            new_velocity.x = direction.x * SPEED;
            new_velocity.z = direction.z * SPEED;
        } else {
            // Gradually reduce the velocity toward zero.
            new_velocity.x = move_toward(new_velocity.x, 0.0f, SPEED);
            new_velocity.z = move_toward(new_velocity.z, 0.0f, SPEED);
        }
        
        // Update the built-in velocity and move.
        set_velocity(new_velocity);

        // Move the character based on the updated velocity.
        move_and_slide();
    }
}
