shader_type spatial;
render_mode specular_schlick_ggx, depth_draw_opaque;

uniform sampler2D noise_texture; // for vertex displacement and optional modulation
uniform sampler2D normal_noise : hint_normal;    // for normal perturbation

// Base color dark green
uniform vec4 base_color = vec4(0.0, 0.3, 0.0, 1.0);

// Material properties
uniform float metallic : hint_range(0.0,1.0) = 0.2;
uniform float roughness : hint_range(0.0,1.0) = 0.2;
uniform float specular : hint_range(0.0,1.0) = 1.0;

// Strength for deformation
uniform float deformation_strength : hint_range(0.0,1.0) = 0.1;

// Tiling for noise textures
uniform float noise_scale : hint_range(0.1,10.0) = 3.0;

// Directional light.
uniform vec3 directional_light_color;
uniform vec3 directional_light_direction;

// Glow properties.
uniform vec3 glow_color = vec3(1.0, 1.0, 1.0);
uniform float glow_intensity : hint_range(0.0,1.0) = 0.5;

uniform vec3 dent_center = vec3(0.0); // Impact point in local space.
uniform float dent_radius = 0.5;       // Radius of influence.
uniform float dent_strength = 0.0;     // How deep the dent is.

uniform bool cool_lighting_on = false;

void vertex() {
    // Scale UVs for noise sampling
    vec2 noise_uv = UV * noise_scale;
    // Sample noise value and offset the vertex along its normal
    float noise_val = texture(noise_texture, noise_uv).r;
    VERTEX += NORMAL * (noise_val - 0.5) * deformation_strength;

	// Calculate distance from the dent center.
    float dist = distance(VERTEX, dent_center);

    // If within the dent radius, apply additional inward displacement.
    if (dist < dent_radius) {
        // 'influence' smoothly drops from 1 at the center to 0 at dent_radius.
        float influence = smoothstep(dent_radius, 0.0, dist);
        // Displace vertex inward along the normal.
        VERTEX -= NORMAL * influence * dent_strength;
    }
}

void fragment() {
    // Set PBR properties.
    METALLIC = metallic;
    ROUGHNESS = roughness;
    SPECULAR = specular;

	// Apply normal map for extra detail.
    	vec3 nmap = texture(normal_noise, UV * noise_scale).rgb;
	nmap = normalize(nmap * 2.0 - 1.0);
    	NORMAL = normalize(mix(NORMAL, nmap, 0.5));

	if (!cool_lighting_on) {
		// Set the base albedo.
    		ALBEDO = base_color.rgb;

		// For EMISSION, use the view angle so surfaces facing the camera glow more.
    		EMISSION = glow_color * glow_intensity;
	}
	else {
		// Normalize the interpolated normal (considering any normal mapping)
    		vec3 norm = normalize(NORMAL);
		// Get the view vector from the built-in VIEW variable.
    		vec3 view_dir = normalize(VIEW);
    		// Calculate the diffuse factor using the directional light's direction.
    		// Note: if your light direction represents the direction the light comes from, you need to invert it.
    		// As the directional light is in world space, you need to convert it to the view space first to make the lighting work properly.
		vec3 light_view = normalize(VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz;
    		float diffuse = dot(norm, light_view);

		// Now compute a view-dependent fresnel term.
    		// This factor will be stronger at grazing angles.
    		float fresnel = sqrt(1.0 - dot(norm, view_dir));

			// Use both the diffuse and fresnel term to compute the ALBEDO.
    		// For example, you can add a slight view-dependent boost:
    		ALBEDO = base_color.rgb * directional_light_color * diffuse +
             base_color.rgb * fresnel * 0.1;

    		// For EMISSION, use the view angle so surfaces facing the camera glow more.
    		EMISSION = glow_color * glow_intensity * fresnel;
	}
}
