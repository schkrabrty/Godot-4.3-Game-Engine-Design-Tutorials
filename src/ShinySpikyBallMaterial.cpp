#include "ShinySpikyBallMaterial.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void ShinySpikyBallMaterial::_bind_methods() {
    // Bind directional light color.
    ClassDB::bind_method(D_METHOD("set_directional_light_color", "color"), &ShinySpikyBallMaterial::set_directional_light_color);
    ClassDB::bind_method(D_METHOD("get_directional_light_color"), &ShinySpikyBallMaterial::get_directional_light_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "directional_light_color"), "set_directional_light_color", "get_directional_light_color");

    // Bind directional light direction.
    ClassDB::bind_method(D_METHOD("set_directional_light_direction", "direction"), &ShinySpikyBallMaterial::set_directional_light_direction);
    ClassDB::bind_method(D_METHOD("get_directional_light_direction"), &ShinySpikyBallMaterial::get_directional_light_direction);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "directional_light_direction"), "set_directional_light_direction", "get_directional_light_direction");

    // Bind glow color.
    ClassDB::bind_method(D_METHOD("set_glow_color", "color"), &ShinySpikyBallMaterial::set_glow_color);
    ClassDB::bind_method(D_METHOD("get_glow_color"), &ShinySpikyBallMaterial::get_glow_color);
    ADD_PROPERTY(PropertyInfo(Variant::COLOR, "glow_color"), "set_glow_color", "get_glow_color");

    // Bind glow intensity.
    ClassDB::bind_method(D_METHOD("set_glow_intensity", "intensity"), &ShinySpikyBallMaterial::set_glow_intensity);
    ClassDB::bind_method(D_METHOD("get_glow_intensity"), &ShinySpikyBallMaterial::get_glow_intensity);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "glow_intensity", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_glow_intensity", "get_glow_intensity");

    // Bind dent center.
    ClassDB::bind_method(D_METHOD("set_dent_center", "dent_center"), &ShinySpikyBallMaterial::set_dent_center);
    ClassDB::bind_method(D_METHOD("get_dent_center"), &ShinySpikyBallMaterial::get_dent_center);
    ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "dent_center"), "set_dent_center", "get_dent_center");

    // Bind dent strength.
    ClassDB::bind_method(D_METHOD("set_dent_strength", "dent_strength"), &ShinySpikyBallMaterial::set_dent_strength);
    ClassDB::bind_method(D_METHOD("get_dent_strength"), &ShinySpikyBallMaterial::get_dent_strength);
    ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "dent_strength", PROPERTY_HINT_RANGE, "0.0,1.0,0.01"), "set_dent_strength", "get_dent_strength");

    // Bind dent center.
    ClassDB::bind_method(D_METHOD("set_lighting_and_reflection", "cool_lighting_on"), &ShinySpikyBallMaterial::set_lighting_and_reflection);
    ClassDB::bind_method(D_METHOD("get_lighting_and_reflection"), &ShinySpikyBallMaterial::get_lighting_and_reflection);
    ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cool_lighting_on"), "set_lighting_and_reflection", "get_lighting_and_reflection");
}

ShinySpikyBallMaterial::ShinySpikyBallMaterial() :
    directional_light_color(Color(1,1,1)),
    directional_light_direction(Vector3(-0.3, -1.0, -0.3)),
    glow_color(Color(1,1,1)),
    glow_intensity(0.5),
    dent_center(Vector3(0,0,0)),
    dent_strength(0.0),
    cool_lighting_on(false)
{
    // Constructor initialization.
    // Initialize the shader parameters with the default values.
    set_shader_parameter("directional_light_color", directional_light_color);
    set_shader_parameter("directional_light_direction", directional_light_direction);
    set_shader_parameter("glow_color", glow_color);
    set_shader_parameter("glow_intensity", glow_intensity);
    set_shader_parameter("dent_center", dent_center);
    set_shader_parameter("dent_strength", dent_strength);
    set_shader_parameter("cool_lighting_on", cool_lighting_on);
}

ShinySpikyBallMaterial::~ShinySpikyBallMaterial() {
    // Destructor cleanup.
}

void ShinySpikyBallMaterial::set_directional_light_color(const Color &p_color) {
    directional_light_color = p_color;
    set_shader_parameter("directional_light_color", directional_light_color);
}

Color ShinySpikyBallMaterial::get_directional_light_color() const {
    return directional_light_color;
}

void ShinySpikyBallMaterial::set_directional_light_direction(const Vector3 &p_direction) {
    directional_light_direction = p_direction;
    set_shader_parameter("directional_light_direction", directional_light_direction);
}

Vector3 ShinySpikyBallMaterial::get_directional_light_direction() const {
    return directional_light_direction;
}

void ShinySpikyBallMaterial::set_glow_color(const Color &p_color) {
    glow_color = p_color;
    set_shader_parameter("glow_color", glow_color);
}

Color ShinySpikyBallMaterial::get_glow_color() const {
    return glow_color;
}

void ShinySpikyBallMaterial::set_glow_intensity(const float &p_intensity) {
    glow_intensity = p_intensity;
    set_shader_parameter("glow_intensity", glow_intensity);
}

float ShinySpikyBallMaterial::get_glow_intensity() const {
    return glow_intensity;
}

void ShinySpikyBallMaterial::set_dent_center(const Vector3 &p_center) {
    dent_center = p_center;
    set_shader_parameter("dent_center", dent_center);
}

Vector3 ShinySpikyBallMaterial::get_dent_center() const {
    return dent_center;
}

void ShinySpikyBallMaterial::set_dent_strength(const float p_strength) {
    dent_strength = p_strength;
    set_shader_parameter("dent_strength", dent_strength);
}

float ShinySpikyBallMaterial::get_dent_strength() const {
    return dent_strength;
}

void ShinySpikyBallMaterial::set_lighting_and_reflection(const bool p_strength) {
    cool_lighting_on = p_strength;
    set_shader_parameter("cool_lighting_on", cool_lighting_on);
}

bool ShinySpikyBallMaterial::get_lighting_and_reflection() const {
    return cool_lighting_on;
}