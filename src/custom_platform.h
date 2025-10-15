#ifndef CUSTOM_PLATFORM_H
#define CUSTOM_PLATFORM_H

#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/classes/physics_material.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {

class CustomPlatform : public StaticBody2D {
    GDCLASS(CustomPlatform, StaticBody2D);

private:
    // This will hold the PhysicsMaterial we assign to the body.
    Ref<PhysicsMaterial> physics_material;

    // Two booleans that you can toggle from UI.
    bool rough;
    bool absorbent;
    float bounce;
    float friction;

protected:
    static void _bind_methods();

public:
    CustomPlatform();
    ~CustomPlatform();

    void _ready() override;

    // Methods to adjust the material properties.
    void set_material_friction(float friction);
    float get_material_friction() const;
    void set_material_bounce(float bounce);
    float get_material_bounce() const;

    // Methods for toggling custom behavior.
    void set_rough(bool rough);
    bool get_rough() const;

    void set_absorbent(bool absorbent);
    bool get_absorbent() const;
};

} // namespace godot

#endif // CUSTOM_PLATFORM_H
