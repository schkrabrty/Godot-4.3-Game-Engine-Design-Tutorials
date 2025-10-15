shader_type spatial;
render_mode blend_mix, depth_draw_opaque, cull_back, diffuse_burley, specular_schlick_ggx;

uniform vec4 albedo : source_color;
uniform sampler2D texture_albedo : source_color, filter_linear_mipmap, repeat_enable;
uniform float point_size : hint_range(0.1, 128.0, 0.1);

uniform float roughness : hint_range(0.0, 1.0);
uniform sampler2D texture_metallic : hint_default_white, filter_linear_mipmap, repeat_enable;
uniform vec4 metallic_texture_channel;
uniform sampler2D texture_roughness : hint_roughness_r, filter_linear_mipmap, repeat_enable;

uniform float specular : hint_range(0.0, 1.0, 0.01);
uniform float metallic : hint_range(0.0, 1.0, 0.01);

uniform sampler2D texture_normal : hint_roughness_normal, filter_linear_mipmap, repeat_enable;
uniform float normal_scale : hint_range(-16.0, 16.0);

uniform sampler2D texture_ambient_occlusion : hint_default_white, filter_linear_mipmap, repeat_enable;
uniform vec4 ao_texture_channel;
uniform float ao_light_affect : hint_range(0.0, 1.0, 0.01);

uniform sampler2D texture_heightmap : hint_default_black, filter_linear_mipmap, repeat_enable;
uniform float heightmap_scale : hint_range(-16.0, 16.0, 0.001);
uniform int heightmap_min_layers : hint_range(1, 64);
uniform int heightmap_max_layers : hint_range(1, 64);
uniform vec2 heightmap_flip;

// New uniform to control vertex displacement strength.
uniform float vertex_disp_scale : hint_range(0.0, 1.0) = 0.2;

uniform vec3 uv1_scale;
uniform vec3 uv1_offset;
uniform vec3 uv2_scale;
uniform vec3 uv2_offset;

uniform vec3 directional_light_color;
uniform vec3 directional_light_direction;

uniform bool cool_lighting_on = false;
uniform bool vertex_shader_on = false;

void vertex() {
    // Adjust UV coordinates.
    UV = UV * uv1_scale.xy + uv1_offset.xy;

	if (vertex_shader_on) {
		// --- Vertex Displacement ---
	    // Sample the height map using the current UV.
	    // Here the red channel is used as the displacement value.
	    float disp = texture(texture_heightmap, UV).r;
	    // Offset the vertex along its normal based on the sampled height.
	    VERTEX += NORMAL * disp * vertex_disp_scale;
	}
}

void fragment() {
    vec2 base_uv = UV;

    // --- Parallax Occlusion Mapping (POM) ---
    // Compute view direction in tangent space.
    vec3 view_dir = normalize(normalize(-VERTEX + EYE_OFFSET) *
      mat3(TANGENT * heightmap_flip.x, -BINORMAL * heightmap_flip.y, NORMAL));

    // Determine the number of layers for raymarching based on view angle.
    float num_layers = mix(float(heightmap_max_layers), float(heightmap_min_layers), abs(dot(vec3(0.0, 0.0, 1.0), view_dir)));
    float layer_depth = 1.0 / num_layers;
    float current_layer_depth = 0.0;
    // Calculate the UV offset per layer.
    vec2 p = view_dir.xy * heightmap_scale * 0.01;
    vec2 delta = p / num_layers;
    vec2 ofs = base_uv;
    // Sample the height map (inverted as used in this shader).
    float depth = 1.0 - texture(texture_heightmap, ofs).r;

    float current_depth = 0.0;
    while (current_depth < depth) {
        ofs -= delta;
        depth = 1.0 - texture(texture_heightmap, ofs).r;
        current_depth += layer_depth;
    }

    // Interpolate between the last two steps for a smoother result.
    vec2 prev_ofs = ofs + delta;
    float after_depth = depth - current_depth;
    float before_depth = (1.0 - texture(texture_heightmap, prev_ofs).r) - current_depth + layer_depth;
    float weight = after_depth / (after_depth - before_depth);
    ofs = mix(ofs, prev_ofs, weight);
    base_uv = ofs;

    // --- Material Property Sampling ---
    float metallic_tex = dot(texture(texture_metallic, base_uv), metallic_texture_channel);
    METALLIC = metallic_tex * metallic;
    SPECULAR = specular;

    vec4 roughness_texture_channel = vec4(1.0, 0.0, 0.0, 0.0);
    float roughness_tex = dot(texture(texture_roughness, base_uv), roughness_texture_channel);
    ROUGHNESS = roughness_tex * roughness;

    NORMAL_MAP = texture(texture_normal, base_uv).rgb;
    NORMAL_MAP_DEPTH = normal_scale;

    AO = dot(texture(texture_ambient_occlusion, base_uv), ao_texture_channel);
    AO_LIGHT_AFFECT = ao_light_affect;

    vec4 albedo_tex = texture(texture_albedo, base_uv);

    if (!cool_lighting_on) {
        ALBEDO = albedo.rgb * albedo_tex.rgb;
    }
    else {
        NORMAL = normalize(mix(NORMAL, NORMAL_MAP, 0.01));
        vec3 norm = normalize(NORMAL);
        vec3 light_view = normalize(VIEW_MATRIX * vec4(directional_light_direction, 0.0)).xyz;
        float diffuse = dot(norm, light_view);
        ALBEDO = albedo.rgb * albedo_tex.rgb * directional_light_color * diffuse;
    }
}
