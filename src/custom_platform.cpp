#include "custom_platform.h"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

CustomPlatform::CustomPlatform() :
    rough(false),
    absorbent(false),
    friction(0.5f),
    bounce(0.3f) {
    // Default constructor – no additional setup needed here.
}

CustomPlatform::~CustomPlatform() {
    // Destructor: cleanup if needed.
}

void CustomPlatform::set_material_friction(float friction) {
    if (physics_material.is_valid()) {
        physics_material->set_friction(friction);
    }
}

float CustomPlatform::get_material_friction() const {
    return friction;
}

void CustomPlatform::set_material_bounce(float bounce) {
    if (physics_material.is_valid()) {
        physics_material->set_bounce(bounce);
    }
}

float CustomPlatform::get_material_bounce() const {
    return bounce;
}

void CustomPlatform::set_rough(bool p_rough) {
    rough = p_rough;
    if (physics_material.is_valid()) {
        // For example, increase friction when rough is true.
        physics_material->set_friction(rough ? 1.0f : 0.5f);
    }
}

bool CustomPlatform::get_rough() const {
    return rough;
}

void CustomPlatform::set_absorbent(bool p_absorbent) {
    absorbent = p_absorbent;
    if (physics_material.is_valid()) {
        // For example, reduce bounce when absorbent is true.
        physics_material->set_bounce(absorbent ? 0.0f : 0.3f);
    }
}

bool CustomPlatform::get_absorbent() const {
    return absorbent;
}

void CustomPlatform::_ready() {
    // // *** Freeze the body ***
    // //
    // // Switching the mode to freeze prevents the physics engine from moving it.
    // set("freeze", true);
    // set("lock_rotation", true);

    // *** Create and assign a PhysicsMaterial ***
    //
    // Instantiate a new PhysicsMaterial.
    physics_material.instantiate();
    if (!physics_material.is_valid()) {
        UtilityFunctions::print("Failed to instantiate PhysicsMaterial.");
        return;
    }
    
    // Set initial friction and bounce values.
    physics_material->set_friction(0.5);
    physics_material->set_bounce(0.3);
    
    // Assign the material via the property system.
    set("physics_material_override", physics_material);
}

void CustomPlatform::_bind_methods() {
    // Bind the methods that can be called from scripts or connected to UI signals.
    ClassDB::bind_method(D_METHOD("set_material_friction", "friction"), &CustomPlatform::set_material_friction);
    ClassDB::bind_method(D_METHOD("get_material_friction"), &CustomPlatform::get_material_friction);
    ClassDB::bind_method(D_METHOD("set_material_bounce", "bounce"), &CustomPlatform::set_material_bounce);
    ClassDB::bind_method(D_METHOD("get_material_bounce"), &CustomPlatform::get_material_bounce);
    ClassDB::bind_method(D_METHOD("set_rough", "rough"), &CustomPlatform::set_rough);
    ClassDB::bind_method(D_METHOD("set_absorbent", "absorbent"), &CustomPlatform::set_absorbent);
    ClassDB::bind_method(D_METHOD("get_rough"), &CustomPlatform::get_rough);
    ClassDB::bind_method(D_METHOD("get_absorbent"), &CustomPlatform::get_absorbent);

    // Expose the booleans as editable properties in the editor.
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "friction"), "set_material_friction", "get_material_friction");
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "bounce"), "set_material_bounce", "get_material_bounce");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "rough"), "set_rough", "get_rough");
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "absorbent"), "set_absorbent", "get_absorbent");
}
