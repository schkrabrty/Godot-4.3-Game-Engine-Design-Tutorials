shader_type spatial;
//render_mode blend_mix, cull_back, depth_draw_opaque;

// Wind and grass uniforms.
uniform float wind_strength : hint_range(0.0, 1.0) = 0.3;
uniform float wind_speed    : hint_range(0.0, 5.0) = 1.0;
uniform vec3 wind_direction = vec3(1.0, 0.0, 0.0);

uniform vec4 base_color = vec4(0.8, 0.3, 0.1, 1.0); // Brownish base
uniform vec4 tip_color  = vec4(0.1, 0.8, 0.1, 1.0); // Greenish tip

uniform float base_y       : hint_range(-1.0, 1.0) = -0.2;
uniform float grass_height : hint_range(0.1, 10.0) = 0.5;

uniform vec3 ball_position;
uniform float ball_influence_distance = 1.5; // Adjusted threshold
uniform float ball_max_bend = 2.0; // Adjusted maximum bend

// New uniforms for directional light (set these from your scene).
uniform vec3 directional_light_color;
uniform vec3 directional_light_direction;

varying float v_rel_height;

uniform bool cool_lighting_on = false;
uniform bool rim_lighting_on = false;

// --- New rim lighting uniforms ---
uniform vec4 rim_color = vec4(1.0, 1.0, 1.0, 1.0); // Color of the rim light
uniform float rim_power : hint_range(0.1, 10.0) = 2.0;            // Controls falloff of the rim
uniform float rim_intensity : hint_range(0.0, 2.0) = 1.0;         // Overall intensity of the rim

void vertex() {
    // Compute relative height and base wind bending.
    float rel_height = clamp((VERTEX.y - base_y) / grass_height, 0.0, 1.0);
    v_rel_height = rel_height;
    float bend_factor = smoothstep(0.0, 1.0, rel_height);
    float angle = wind_strength * sin(TIME * wind_speed) * bend_factor;

    vec3 base = vec3(0.0, base_y, 0.0);
    vec3 v = VERTEX - base;

    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 rot_axis = normalize(cross(up, wind_direction));

    vec3 rotated = v * cos(angle) +
                   cross(rot_axis, v) * sin(angle) +
                   rot_axis * dot(rot_axis, v) * (1.0 - cos(angle));

    // NEW: Compute ball repulsion effect.
    vec3 world_pos = (MODEL_MATRIX * vec4(VERTEX, 1.0)).xyz;
    float d = distance(world_pos, ball_position);
    float ball_influence = 1.0 - smoothstep(0.0, ball_influence_distance, d);

    vec3 ball_dir = normalize(world_pos - ball_position);
    vec3 repulsion_rot_axis = normalize(cross(up, ball_dir));
    float repulsion_angle = ball_influence * ball_max_bend;

    vec3 repulsion_rotated = v * cos(repulsion_angle) +
                             cross(repulsion_rot_axis, v) * sin(repulsion_angle) +
                             repulsion_rot_axis * dot(repulsion_rot_axis, v) * (1.0 - cos(repulsion_angle));

    // Blend wind and ball repulsion effects based on ball proximity.
    VERTEX = base + mix(rotated, repulsion_rotated, ball_influence);
}

void fragment() {
    // Blend from brown at the base to green at the tip.
    vec3 gradient = mix(base_color.rgb, tip_color.rgb, v_rel_height);

	// Normalize the interpolated normal (considering any normal mapping)
    vec3 norm = normalize(NORMAL);
	// Get the view vector from the built-in VIEW variable.
    vec3 view_dir = normalize(VIEW);

	if (!cool_lighting_on) {
		ALBEDO = gradient;
	}
	else {
		// Calculate the diffuse factor using the directional light's direction.
	    // Note: if your light direction represents the direction the light comes from, you need to invert it.
	    // As the directional light is in world space, you need to convert it to the view space first to make the lighting work properly.
		vec3 light_view = normalize(VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz;
	    float diffuse = dot(norm, light_view);

		if (!rim_lighting_on) {
			ALBEDO = gradient * directional_light_color * diffuse;
		}
		else {
			// The rim factor increases when the surface normal is nearly perpendicular to the light.
			float rim = 1.0 - max(diffuse, 0.0);
			rim = pow(rim, rim_power) * rim_intensity; // Adjust exponent for sharper or softer rim effect.
			ALBEDO = gradient * directional_light_color * diffuse + rim * rim_color.rgb;
		}
	}
}
