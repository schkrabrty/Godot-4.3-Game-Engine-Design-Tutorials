shader_type spatial;
render_mode specular_schlick_ggx, depth_draw_opaque;

uniform sampler2D noise_texture;
uniform sampler2D normal_noise : hint_normal;

uniform vec4 base_color = vec4(0.0, 0.3, 0.0, 1.0);

uniform float metallic : hint_range(0.0,1.0) = 0.2;
uniform float roughness : hint_range(0.0,1.0) = 0.2;
uniform float specular : hint_range(0.0,1.0) = 1.0;

uniform float deformation_strength : hint_range(0.0,1.0) = 0.1;
uniform float noise_scale : hint_range(0.1,10.0) = 3.0;

uniform vec3 directional_light_color = vec3(1.0);
uniform vec3 directional_light_direction = vec3(1.0);

uniform vec3 glow_color = vec3(1.0, 1.0, 1.0);
uniform float glow_intensity : hint_range(0.0,1.0) = 0.5;

uniform bool cool_lighting_on = false;
uniform bool explosion_on = false;

// New uniforms for controlling the effect externally.
uniform float explosion_time = 0.0; // <-- Controlled by script (reset to 0 each time)
uniform float explosion_speed : hint_range(0.0,10.0) = 0.5;
uniform float dissolve_delay   : hint_range(0.0,10.0) = 1.0;
uniform float dissolve_speed   : hint_range(0.0,10.0) = 0.5;

void vertex() {
    vec2 noise_uv = UV * noise_scale;
    float noise_val = texture(noise_texture, noise_uv).r;
    VERTEX += NORMAL * (noise_val - 0.5) * deformation_strength;

    if (explosion_on) {
        float local_time = explosion_time;
        float explosion_strength = clamp(local_time * explosion_speed, 0.0, 1.0);
        VERTEX += NORMAL * explosion_strength;
    }
}

void fragment() {
    if (explosion_on) {
        float local_time = explosion_time;
        float t = max(local_time - dissolve_delay, 0.0);
        float dissolve_threshold = clamp(t * dissolve_speed, 0.0, 1.0);
        vec2 dissolve_uv = UV * noise_scale;
        float noise_value_dissolve = texture(noise_texture, dissolve_uv).r;
        if (noise_value_dissolve < dissolve_threshold) {
            discard;
        }
    }

    METALLIC = metallic;
    ROUGHNESS = roughness;
    SPECULAR = specular;

    vec3 nmap = texture(normal_noise, UV * noise_scale).rgb;
    nmap = normalize(nmap * 2.0 - 1.0);
    NORMAL = normalize(mix(NORMAL, nmap, 0.5));

    if (!cool_lighting_on) {
        ALBEDO = base_color.rgb;
        EMISSION = glow_color * glow_intensity;
    } else {
        vec3 norm = normalize(NORMAL);
        vec3 view_dir = normalize(VIEW);
        vec3 light_view = normalize((VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz);
        float diffuse = dot(norm, light_view);
        float fresnel = sqrt(1.0 - dot(norm, view_dir));
        ALBEDO = base_color.rgb * directional_light_color * diffuse +
                 base_color.rgb * fresnel * 0.1;
        EMISSION = glow_color * glow_intensity * fresnel;
    }
}
