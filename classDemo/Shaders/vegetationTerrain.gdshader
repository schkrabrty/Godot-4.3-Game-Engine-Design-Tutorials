shader_type spatial;

uniform sampler2D noise;
uniform float height_scale = 0.2;
uniform sampler2D normalmap;
varying vec2 tex_position;
uniform sampler2D grass_texture;
uniform float texture_scale = 100.0; // adjust to tile the texture appropriately
uniform float slope_factor = 2.0; // overall steepness
uniform float slope_start = -8.0; // where the slope starts (z value)
uniform float slope_end = 8.0;    // where the full slope effect is reached

// New uniforms for directional light (set these from your scene).
uniform vec3 directional_light_color;
uniform vec3 directional_light_direction;

uniform bool cool_lighting_on = false;

void vertex() {
	tex_position = VERTEX.xz / 2.0 + 0.5;
	float height = texture(noise, tex_position).x;
	VERTEX.y += height * height_scale;

	// Instead of subtracting a linear term, use smoothstep to create a curved, natural slope.
    // This creates a value that smoothly ramps from 0 to 1 as z goes from slope_start to slope_end.
    float curved_slope = smoothstep(slope_start, slope_end, VERTEX.z);
    VERTEX.y -= curved_slope * slope_factor;
}

void fragment() {
	// Called for every pixel the material is visible on.
	NORMAL_MAP = texture(normalmap, tex_position).xyz;
	// Tile the texture over the terrain using UVs scaled by texture_scale
    vec2 tiled_uv = UV * texture_scale;
    vec3 texture_color = texture(grass_texture, tiled_uv).rgb;
	NORMAL = normalize(mix(NORMAL, NORMAL_MAP, 0.01));
	// Normalize the interpolated normal (considering any normal mapping)
    vec3 norm = normalize(NORMAL);
	// Get the view vector from the built-in VIEW variable.
    vec3 view_dir = normalize(VIEW);

	if (!cool_lighting_on) {
		ALBEDO = texture_color.rgb;
	}
	else {
		// Calculate the diffuse factor using the directional light's direction.
	    // Note: if your light direction represents the direction the light comes from, you need to invert it.
	    // As the directional light is in world space, you need to convert it to the view space first to make the lighting work properly.
		vec3 light_view = normalize(VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz;
	    float diffuse = dot(norm, light_view);
		ALBEDO = texture_color.rgb * directional_light_color * diffuse;
	}
}

//void light() {
	// Called for every pixel for every light affecting the material.
	// Uncomment to replace the default light processing function with this one.
//}
