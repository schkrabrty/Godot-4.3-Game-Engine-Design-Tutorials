#ifndef WIND_2D_H
#define WIND_2D_H

#include <godot_cpp/classes/area2d.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

class WindArea2D : public Area2D {
    GDCLASS(WindArea2D, Area2D);

private:
    // Wind properties exposed to the editor.
    Vector2 wind_direction;    // The base wind direction (normalized).
    float wind_speed;          // Base wind speed.
    float gust_intensity;      // How strong the gusts are.
    float gust_frequency;      // How often gusts occur.
    float air_density;         // Air density used in force calculations (kg/m³).

    // Accumulated time (in seconds) used to compute gusts.
    double accumulated_time;

protected:
    // Register methods and properties with Godot.
    static void _bind_methods();

public:
    WindArea2D();
    ~WindArea2D();

    // Called each physics frame.
    void _physics_process(double delta) override;

    // Setters and getters for wind_direction.
    void set_wind_direction(const Vector2 &p_direction);
    Vector2 get_wind_direction() const;

    // Setters and getters for wind_speed.
    void set_wind_speed(float p_speed);
    float get_wind_speed() const;

    // Setters and getters for gust_intensity.
    void set_gust_intensity(float p_intensity);
    float get_gust_intensity() const;

    // Setters and getters for gust_frequency.
    void set_gust_frequency(float p_frequency);
    float get_gust_frequency() const;

    // Setters and getters for air_density.
    void set_air_density(float p_density);
    float get_air_density() const;
};

#endif // WIND_2D_H
