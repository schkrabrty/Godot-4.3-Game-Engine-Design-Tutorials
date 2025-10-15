shader_type spatial;

uniform vec3 axis_scale = vec3(1.0, 1.0, 1.0);
uniform sampler2D noise;
uniform float height_scale = 0.2;
uniform sampler2D normalmap;

// Dark near-black at the base, and a richer, deeper brown for the top.
uniform vec3 base_color = vec3(0.1, 0.1, 0.1);
uniform vec3 top_color = vec3(0.8, 0.4, 0.2);

varying vec2 tex_position;
varying float v_height;

// New uniforms for directional light (set these from your scene).
uniform vec3 directional_light_color;
uniform vec3 directional_light_direction;

uniform bool cool_lighting_on = false;

void vertex() {
	// Apply non-uniform scaling.
    VERTEX.x *= axis_scale.x;
    VERTEX.y *= axis_scale.y;
    VERTEX.z *= axis_scale.z;

    // Generate spherical UV coordinates.
    float u = atan(VERTEX.z, VERTEX.x) / (2.0 * 3.14159) + 0.5;
    float v = asin(clamp(VERTEX.y / length(VERTEX), -1.0, 1.0)) / 3.14159 + 0.5;
    tex_position = vec2(u, v);
    v_height = v;

    // Calculate a blend factor (0.0 at the bottom, 1.0 above the threshold).
    float blend = smoothstep(0.2, 0.25, v_height);

    // Sample noise and apply displacement scaled by blend factor.
    float n = texture(noise, tex_position).r;
    VERTEX += NORMAL * n * height_scale * blend;

    // Ensure the bottom is completely flat.
    if (v_height < 0.4) {
        VERTEX.y = 0.0;
    }
}

void fragment() {
	float t = smoothstep(0.4, 1.2, v_height);
    vec3 base = mix(base_color, top_color, t);

	// Continue applying your normal map.
    NORMAL_MAP = texture(normalmap, tex_position).rgb;
    NORMAL = normalize(mix(NORMAL, NORMAL_MAP, 0.01));

    // Normalize the interpolated normal (considering any normal mapping)
    vec3 norm = normalize(NORMAL);
	// Get the view vector from the built-in VIEW variable.
    vec3 view_dir = normalize(VIEW);

	if (!cool_lighting_on) {
		ALBEDO = base;
	}
	else {
		// Calculate the diffuse factor using the directional light's direction.
    // Note: if your light direction represents the direction the light comes from, you need to invert it.
	// As the directional light is in world space, you need to convert it to the view space first to make the lighting work properly.
	vec3 light_view = normalize(VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz;
    float diffuse = dot(norm, light_view);
	ALBEDO = base * directional_light_color * diffuse;
	}
}
