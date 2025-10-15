shader_type spatial;
render_mode unshaded, depth_draw_opaque, cull_front;

uniform sampler2D noise_texture;
uniform sampler2D normal_noise : hint_normal;

// Same parameters as in your main shader.
uniform float noise_scale : hint_range(0.1, 10.0) = 3.0;
uniform float deformation_strength : hint_range(0.0, 1.0) = 0.1;

// Outline-specific parameters.
uniform float outline_thickness : hint_range(0.0, 0.1) = 0.03;
uniform vec4 outline_color = vec4(0.0, 0.0, 0.0, 1.0);

void vertex() {
    // Compute noise-based deformation.
    vec2 noise_uv = UV * noise_scale;
    float noise_val = texture(noise_texture, noise_uv).r;

    // Sample and process the normal noise.
    vec3 nmap = texture(normal_noise, UV * noise_scale).rgb;
    nmap = normalize(nmap * 2.0 - 1.0);

    // Compute the perturbed normal similar to the main shader.
    vec3 perturbed_normal = normalize(mix(NORMAL, nmap, 0.5));

    // Apply the same noise deformation.
    VERTEX += perturbed_normal * (noise_val - 0.5) * deformation_strength;

	// Then offset along the perturbed normal for the outline.
    VERTEX += perturbed_normal * outline_thickness;
}

void fragment() {
    ALBEDO = outline_color.rgb;
}
