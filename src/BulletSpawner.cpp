#include "BulletSpawner.h"

using namespace godot; 

BulletSpawner::BulletSpawner() {
}

BulletSpawner::~BulletSpawner() {
}

void BulletSpawner::set_speed(float p_speed) {
    speed = p_speed;
}

float BulletSpawner::get_speed() const {
    return speed;
}

void BulletSpawner::_bind_methods() {
    // Bind the spawn_bullet method.
    ClassDB::bind_method(D_METHOD("spawn_bullet", "bullet_transform"), &BulletSpawner::spawn_bullet);
    
    // Bind getter and setter for the 'speed' property.
    ClassDB::bind_method(D_METHOD("set_speed", "speed"), &BulletSpawner::set_speed);
    ClassDB::bind_method(D_METHOD("get_speed"), &BulletSpawner::get_speed);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "speed"), "set_speed", "get_speed");
}

void BulletSpawner::_ready() {
    // Cache the muzzle node using its NodePath.
    muzzle = get_node<Node3D>(NodePath("Barrel/Muzzle"));

    // Cache the bullet scene once.
    bullet_scene_cache = ResourceLoader::get_singleton()->load("res://Scenes/Networking with CPP/bullet_cpp.tscn");
    if (!bullet_scene_cache.is_valid()) {
        UtilityFunctions::print("Failed to load bullet scene in _ready!");
    }

    // RPC configuration for spawn_bullet.
    Dictionary config;
    config["mode"] = MultiplayerAPI::RPCMode::RPC_MODE_ANY_PEER;
    rpc_config("spawn_bullet", config);
}

void BulletSpawner::spawn_bullet(const Transform3D &bullet_transform) {
    // Use the cached bullet scene.
    if (!bullet_scene_cache.is_valid()) {
        UtilityFunctions::print("Cached bullet scene is invalid!");
        return;
    }

    Node *bullet = bullet_scene_cache->instantiate();
    if (!bullet) {
        UtilityFunctions::print("Failed to instantiate bullet from cached scene!");
        return;
    }

    bullet->set_multiplayer_authority(get_multiplayer()->get_unique_id());

    // Add the bullet to the current scene.
    SceneTree *tree = get_tree();
    if (tree && tree->get_current_scene()) {
        tree->get_current_scene()->add_child(bullet);
    } else {
        UtilityFunctions::print("Current scene not found!");
    }

    // Set the bullet's global transform.
    bullet->set("global_transform", bullet_transform);

    // Adjust the bullet's scale.
    Vector3 bullet_scale = bullet->get("scale");
    bullet_scale.x *= 6.0f;
    bullet_scale.y *= 6.0f;
    bullet_scale.z *= 2.0f;
    bullet->set("scale", bullet_scale);

    // Set the bullet's linear velocity using its forward direction (-basis.z).
    Vector3 linear_velocity = speed * (-bullet_transform.basis.get_column(2));
    bullet->set("linear_velocity", linear_velocity);

    // Call the bullet's method to start its lifetime.
    bullet->call("start_lifetime", 2);
}

void BulletSpawner::_physics_process(double delta) {    
    // If this peer is authoritative and the "shoot" action is pressed...
    if (is_multiplayer_authority() && Input::get_singleton()->is_action_just_pressed("shoot")) {
        if (muzzle) {
            // Direct call.
            spawn_bullet(muzzle->get_global_transform());
            // For networked execution, use RPC so that all peers execute spawn_bullet.
            rpc("spawn_bullet", muzzle->get_global_transform());
        }
    }
}
