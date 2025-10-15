#ifndef MYCHARACTER_H
#define MYCHARACTER_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/camera3d.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/core/math.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

class MyCharacter : public CharacterBody3D {
    GDCLASS(MyCharacter, CharacterBody3D);

private:
    static constexpr float SPEED = 5.0f;
    static constexpr float JUMP_VELOCITY = 4.5f;
    Vector3 velocity; // Member variable for velocity.

    // Exported sensitivity property.
    float sensitivity = 0.5f;

    // Cached nodes.
    Node3D *pivot = nullptr;    // Expected at "CameraOrigin"
    Camera3D *cam = nullptr;      // Expected at "CameraOrigin/SpringArm3D/Camera3D"

protected:
    static void _bind_methods();

public:
    MyCharacter();
    ~MyCharacter();

    // Lifecycle methods.
    void _enter_tree() override;
    void _ready() override;
    void _input(const Ref<InputEvent> &event) override;
    void _physics_process(double delta) override;

    // Getter/Setter for the sensitivity property.
    void set_sensitivity(float s);
    float get_sensitivity() const;
};

} // namespace godot

#endif // MYCHARACTER_H
