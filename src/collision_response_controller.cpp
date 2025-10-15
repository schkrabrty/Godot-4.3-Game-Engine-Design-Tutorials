#include "collision_response_controller.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/physics_direct_body_state2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

using namespace godot;

void CollisionResponseController::_bind_methods() {
    // Expose create_dent() so you can call it from scripts if needed.
    ClassDB::bind_method(D_METHOD("create_dent"), &CollisionResponseController::create_dent);

    // Expose the impact_threshold property.
    ClassDB::bind_method(D_METHOD("set_impact_threshold", "value"), &CollisionResponseController::set_impact_threshold);
    ClassDB::bind_method(D_METHOD("get_impact_threshold"), &CollisionResponseController::get_impact_threshold);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "impact_threshold"), "set_impact_threshold", "get_impact_threshold");

    // Bind the simulate_dent method if you need to call it directly from scripts.
    ClassDB::bind_method(D_METHOD("simulate_dent", "impact_point", "dent_strength", "dent_radius"),
                         &CollisionResponseController::simulate_dent);

    // Bind the is_player setter/getter.
    ClassDB::bind_method(D_METHOD("set_is_player", "value"), &CollisionResponseController::set_is_player);
    ClassDB::bind_method(D_METHOD("get_is_player"), &CollisionResponseController::get_is_player);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_player"), "set_is_player", "get_is_player");
}

CollisionResponseController::CollisionResponseController() {
    // Disable gravity by setting the gravity scale to 0.
    set_gravity_scale(0.0f);

    // Enable contact monitoring.
    set_contact_monitor(true);

    // Set the maximum number of contacts to be reported.
    set_max_contacts_reported(10);
}

CollisionResponseController::~CollisionResponseController() {
    // (Optional) Cleanup code.
}

void CollisionResponseController::set_impact_threshold(float p_value) {
    impact_threshold = p_value;
}

float CollisionResponseController::get_impact_threshold() const {
    return impact_threshold;
}

void CollisionResponseController::set_is_player(bool p_value) {
    is_player = p_value;
}

bool CollisionResponseController::get_is_player() const {
    return is_player;
}

void CollisionResponseController::_integrate_forces(PhysicsDirectBodyState2D *state) {
    // Get the physics delta time.
    float dt = state->get_step();
    // Update the cooldown timer.
    if (dent_cooldown_timer > 0.0f) {
        dent_cooldown_timer -= dt;
    }

    int contact_count = state->get_contact_count();
    // UtilityFunctions::print("Contact count: " + String::num_int64(contact_count));

    // Get our current velocity.
    Vector2 my_velocity = state->get_linear_velocity();

    for (int i = 0; i < contact_count; i++) {
        // Obtain the collider's velocity at the contact point.
        Vector2 collider_velocity = state->get_contact_collider_velocity_at_position(i);
        // Compute the relative (impact) velocity.
        Vector2 relative_velocity = my_velocity - collider_velocity;
        float impact_speed = relative_velocity.length();

        // Only process a collision if impact speed is high enough AND our cooldown timer has expired.
        if (impact_speed >= impact_threshold && dent_cooldown_timer <= 0.0f) {
            // Log the impact (this is where you’d add a visual dent or change a shader parameter).
            UtilityFunctions::print("Impact speed: " + String::num_real(impact_speed) +
                                      " exceeded threshold on: " + get_name());

            // Apply a dent to this body.
            create_dent();
            // Reset the cooldown timer.
            dent_cooldown_timer = DENT_COOLDOWN_DURATION;

            // If the other body is also a CollisionResponseController, apply a dent there.
            Object *collider_obj = state->get_contact_collider_object(i);
            if (collider_obj) {
                CollisionResponseController *other = Object::cast_to<CollisionResponseController>(collider_obj);
                if (other && other->dent_cooldown_timer <= 0.0f) {
                    other->create_dent();
                    other->dent_cooldown_timer = DENT_COOLDOWN_DURATION;
                }
            }
        }
    }
}

void CollisionResponseController::create_dent() {
    // Increase the damage counter.
    collision_count++;

    // Get the Sprite2D child node.
    Sprite2D *sprite = get_node<Sprite2D>(NodePath("Sprite2D"));
    if (!sprite) {
        UtilityFunctions::print("Sprite2D not found on: " + get_name());
        return;
    }

    // If this is the player, reduce the sprite's scale to 0.24.
    String node_name = get_name();
    if (node_name == String("Player")) {
        is_player = true;
    }

    if (is_player) {
        sprite->set_scale(Vector2(0.24, 0.24));
    }

    if (collision_count <= 5) {
        // Build the texture path based on whether this is the player or enemy.
        String texture_path;
        if (is_player) {
            texture_path = "res://Images/Deformed_Images/Player/icon_deformed_" + String::num_int64(collision_count) + ".png";
        } else {
            texture_path = "res://Images/Deformed_Images/Enemy/Enemy_deformed_" + String::num_int64(collision_count) + ".png";
        }
        // UtilityFunctions::print("Loading texture: " + texture_path);
        Ref<Texture2D> new_texture = ResourceLoader::get_singleton()->load(texture_path);
        if (new_texture.is_valid()) {
            sprite->set_texture(new_texture);
        } else {
            UtilityFunctions::print("Failed to load texture at: " + texture_path);
        }
    } else {
        // UtilityFunctions::print(String(get_name()) + " has reached maximum damage and will be destroyed.");
        // When collisions exceed 5, remove this rigidbody from the scene.
        queue_free();
    }
    
    // ----------------------------------------------------------------
    // Simulate a dent on the CollisionPolygon2D if needed.
    // ----------------------------------------------------------------

    // For demonstration, we simulate a dent at a fixed local point.
    // In a real scenario, you might compute this point from the collision information.
    // Vector2 impact_point = Vector2(0, 0); // Local coordinates (adjust as needed)
    // float dent_strength = 10.0f;          // How many pixels to move the vertices inward
    // float dent_radius = 10.0f;            // Radius (in pixels) around the impact point to affect

    // UtilityFunctions::print("Creating dent on: " + get_name());
    // simulate_dent(impact_point, dent_strength, dent_radius);
}

void CollisionResponseController::simulate_dent(const Vector2 &impact_point, float dent_strength, float dent_radius) {
    // Get the CollisionPolygon2D child node.
    CollisionPolygon2D *cp = get_node<CollisionPolygon2D>(NodePath("CollisionPolygon2D"));
    if (!cp) {
        UtilityFunctions::print("CollisionPolygon2D not found for dent simulation on: " + get_name());
        return;
    }

    // Get the current polygon (list of points).
    PackedVector2Array poly = cp->get_polygon();

    // Loop through each vertex and adjust it if it’s within the dent radius.
    for (int i = 0; i < poly.size(); i++) {
        Vector2 vertex = poly[i];
        float distance = vertex.distance_to(impact_point);
        if (distance < dent_radius) {
            // Compute a normalized direction from the impact point to the vertex.
            Vector2 direction = (vertex - impact_point).normalized();
            // Calculate an offset factor that is larger the closer the vertex is to the impact point.
            float factor = dent_strength * (1.0f - (distance / dent_radius));
            // Clamp the factor to the dent strength.
            factor = Math::clamp<float>(factor, 0.0f, dent_strength);
            // Move the vertex inward.
            vertex -= direction * factor;
            poly.set(i, vertex);
        }
    }
    // Update the collision polygon with the modified points.
    cp->call_deferred("set_polygon", poly);
}
