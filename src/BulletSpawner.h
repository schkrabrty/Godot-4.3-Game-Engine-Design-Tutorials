#ifndef BULLETSPAWNER_H
#define BULLETSPAWNER_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/variant/transform3d.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/multiplayer_api.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/classes/resource_loader.hpp> // For ResourceLoader
#include <godot_cpp/classes/packed_scene.hpp>    // For PackedScene

namespace godot {

class BulletSpawner : public Node3D {
    GDCLASS(BulletSpawner, Node3D);

private:
    // Exported property for speed.
    float speed = 40.0f;
    
    // Cached node for the muzzle.
    Node3D *muzzle = nullptr;

    Ref<PackedScene> bullet_scene_cache; // Cached bullet scene

protected:
    static void _bind_methods();

public:
    BulletSpawner();
    ~BulletSpawner();

    // Lifecycle callback.
    void _ready() override;

    // Called every physics frame.
    void _physics_process(double delta) override;

    // RPC method to spawn a bullet.
    void spawn_bullet(const Transform3D &bullet_transform);

    // Getter/Setter functions for 'speed'.
    void set_speed(float p_speed);
    float get_speed() const;
};

} // namespace godot

#endif // BULLETSPAWNER_H
