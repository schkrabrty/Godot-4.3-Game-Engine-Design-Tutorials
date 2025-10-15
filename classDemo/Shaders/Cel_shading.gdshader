shader_type spatial;
render_mode unshaded, depth_draw_opaque;

// Cel shading and deformation pass.
uniform sampler2D noise_texture;
uniform sampler2D normal_noise : hint_normal;
//uniform sampler2D height_map; // Height map for parallax mapping.
uniform vec4 base_color = vec4(0.0, 0.8, 0.0, 1.0);
uniform float deformation_strength : hint_range(0.0, 1.0) = 0.1;
uniform float noise_scale : hint_range(0.1, 10.0) = 3.0;
//uniform float parallax_scale : hint_range(0.0, 0.1) = 0.02; // Parallax occlusion mapping parameter.
uniform vec3 directional_light_color = vec3(1.0);
uniform vec3 directional_light_direction = vec3(0.0, 1.0, 0.0);
uniform int toon_levels : hint_range(1,20) = 5;
uniform float shadow_bias : hint_range(0.0, 1.0) = 0.001;
uniform bool cool_lighting_on = false;

void vertex() {
    vec2 noise_uv = UV * noise_scale;
    float noise_val = texture(noise_texture, noise_uv).r;

    // Perturbed normal from normal map.
    vec3 nmap = texture(normal_noise, UV * noise_scale).rgb;
    nmap = normalize(nmap * 2.0 - 1.0);
    vec3 perturbed_normal = normalize(mix(NORMAL, nmap, 0.5));

    // Apply noise-based deformation.
    VERTEX += perturbed_normal * (noise_val - 0.5) * deformation_strength;
}

void fragment() {
	// Start with the original UV coordinates.
    vec2 finalUV = UV;

    // Cel-shading: sample and perturb the normal.
    vec3 nmap = texture(normal_noise, finalUV * noise_scale).rgb;
    nmap = normalize(nmap * 2.0 - 1.0);
    vec3 norm = normalize(mix(NORMAL, nmap, 0.5));

    // Light direction in view space.
    vec3 light_view = normalize((VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz);
    float diff = max(dot(norm, light_view), 0.0);
    diff = (diff - shadow_bias) / (1.0 - shadow_bias);
    diff = clamp(diff, 0.0, 1.0);
    float toon_diff = floor(diff * float(toon_levels)) / float(toon_levels);

    ALBEDO = !cool_lighting_on
        ? base_color.rgb * toon_diff
        : base_color.rgb * directional_light_color * toon_diff;
}
