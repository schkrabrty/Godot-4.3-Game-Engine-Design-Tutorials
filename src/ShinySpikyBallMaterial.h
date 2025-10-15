#ifndef SHINY_SPIKY_BALL_MATERIAL_H
#define SHINY_SPIKY_BALL_MATERIAL_H

#include <godot_cpp/classes/shader_material.hpp>
#include <godot_cpp/variant/color.hpp>
#include <godot_cpp/variant/vector3.hpp>

namespace godot {

class ShinySpikyBallMaterial : public ShaderMaterial {
    GDCLASS(ShinySpikyBallMaterial, ShaderMaterial);

private:
    // Directional light parameters.
    Color directional_light_color;
    Vector3 directional_light_direction;
    
    // Glow properties.
    Color glow_color;
    float glow_intensity;
    Vector3 dent_center;
    float dent_strength;
    bool cool_lighting_on;

protected:
    static void _bind_methods();

public:
    ShinySpikyBallMaterial();
    ~ShinySpikyBallMaterial();

    // Directional light color.
    void set_directional_light_color(const Color &p_color);
    Color get_directional_light_color() const;

    // Directional light direction.
    void set_directional_light_direction(const Vector3 &p_direction);
    Vector3 get_directional_light_direction() const;

    // Glow color.
    void set_glow_color(const Color &p_color);
    Color get_glow_color() const;

    // Glow intensity.
    void set_glow_intensity(const float &p_intensity);
    float get_glow_intensity() const;

    // Dent Center.
    void set_dent_center(const Vector3 &p_center);
    Vector3 get_dent_center() const;

    // Dent Strength.
    void set_dent_strength(const float p_strength);
    float get_dent_strength() const;

    // Lighting and Reflection.
    void set_lighting_and_reflection(const bool p_strength);
    bool get_lighting_and_reflection() const;
};

} // namespace godot

#endif // SHINY_SPIKY_BALL_MATERIAL_H
