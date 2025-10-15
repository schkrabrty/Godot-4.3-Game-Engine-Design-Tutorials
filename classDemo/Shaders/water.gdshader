shader_type spatial;
render_mode specular_toon;
//render_mode specular_schlick_ggx;
//render_mode specular_disabled;

// Textures for displacement and normal detail.
uniform sampler2D noise;
uniform sampler2D normalmap;
uniform float height_scale = 0.5;
varying vec2 tex_position;

// Wind parameters.
uniform float wind_strength : hint_range(0.0, 1.0) = 0.3;
uniform float wind_speed    : hint_range(0.0, 5.0) = 1.0;
uniform vec3 wind_direction = vec3(1.0, 0.0, 0.0);

// Directional light.
uniform vec3 directional_light_color;
uniform vec3 directional_light_direction;

// PBR water properties.
uniform float water_roughness : hint_range(0.0,1.0) = 0.01; // very smooth surface
uniform float water_specular  : hint_range(0.0,1.0) = 1.0;
uniform float water_metallic  : hint_range(0.0,1.0) = 0.0;  // water is non-metallic
//uniform vec3 water_base_color = vec3(0.01, 0.03, 0.05); // darker water base color
uniform vec3 water_base_color = vec3(0.1, 0.3, 0.5); // darker water base color

// New uniform for refraction strength.
// This controls how far the refraction offset moves the screen sample.
uniform float refraction_strength : hint_range(0.0, 1.0) = 0.05;
uniform float water_color_strength : hint_range(0.0, 20.0) = 10.0;

//uniform sampler2D screen_texture : hint_screen_texture, repeat_disable, filter_nearest;
uniform sampler2D screen_texture : hint_screen_texture, filter_linear_mipmap;
//uniform sampler2D screen_texture : hint_screen_texture, repeat_disable, filter_linear_mipmap;

uniform bool cool_lighting_on = false;
uniform bool reflection_on = false;

float wave(vec2 position) {
    position += texture(noise, position / 10.0).x * 2.0 - 1.0;
    vec2 wv = 1.0 - abs(sin(position));
    return pow(1.0 - pow(wv.x * wv.y, 0.65), 4.0);
}

float base_height(vec2 position, float time) {
    vec2 windDir = normalize(vec2(wind_direction.x, wind_direction.z));
    vec2 wind_offset = windDir * wind_speed * wind_strength * (time * 0.25);
    float d = wave((position + wind_offset) * 0.4) * 0.3;
    d += wave((position - wind_offset) * 0.3) * 0.3;
    d += wave((position + wind_offset) * 0.5) * 0.2;
    d += wave((position - wind_offset) * 0.6) * 0.2;
    return d;
}

void vertex() {
    vec2 pos = VERTEX.xz;
    float k_base = base_height(pos, TIME) * height_scale;
    float k = k_base;
    // Compute normals using finite differences.
    float eps = 0.1;
    float h_dx = k - (base_height(pos + vec2(eps, 0.0), TIME) * height_scale);
    float h_dz = k - (base_height(pos + vec2(0.0, eps), TIME) * height_scale);
    NORMAL = normalize(vec3(h_dx, eps, h_dz));
    VERTEX.y = k;
	tex_position = VERTEX.xz / 2.0 + 0.5;
}

void fragment() {
	NORMAL_MAP = texture(normalmap, tex_position).xyz;
	NORMAL = normalize(mix(NORMAL, NORMAL_MAP, 0.01));

	// Normalize the interpolated normal (considering any normal mapping)
	vec3 norm = normalize(NORMAL);
	// Get the view vector from the built-in VIEW variable.
	vec3 view_dir = normalize(VIEW);

	// Now compute a view-dependent fresnel term.
	// This factor will be stronger at grazing angles.
	float fresnel = sqrt(1.0 - dot(norm, view_dir));

	if (!cool_lighting_on) {
		// Use both the diffuse and fresnel term to compute the ALBEDO.
	    // For example, you can add a slight view-dependent boost:

		if (!reflection_on) {
			ALBEDO = water_base_color * fresnel * 0.1;
		}
		else {
			vec3 water_lit = water_base_color * fresnel * 0.1;
			vec3 refract_color = texture(screen_texture, SCREEN_UV + (NORMAL.xy * refraction_strength)).rgb;
			vec3 final_color = water_lit * refract_color * water_color_strength;
			ALBEDO = final_color;
		}
	}
	else {
	    // Calculate the diffuse factor using the directional light's direction.
	    // Note: if your light direction represents the direction the light comes from, you need to invert it.
	    // As the directional light is in world space, you need to convert it to the view space first to make the lighting work properly.
		vec3 light_view = normalize(VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz;
	    float diffuse = dot(norm, light_view);
		vec3 water_lit = water_base_color * directional_light_color * diffuse + water_base_color * fresnel * 0.1;

		if (!reflection_on) {
			ALBEDO = water_lit;
		}
		else {
			vec3 refract_color = texture(screen_texture, SCREEN_UV + (NORMAL.xy * refraction_strength)).rgb;
			vec3 final_color = water_lit * refract_color * water_color_strength;
			ALBEDO = final_color;
		}
	}

	METALLIC = water_metallic;
    SPECULAR = water_specular;
	ROUGHNESS = water_roughness * (1.0 - fresnel);
}
