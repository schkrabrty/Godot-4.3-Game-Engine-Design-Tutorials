extends Node3D

@onready var directionalLight := $DirectionalLight3D
@onready var terrain := $Terrain

@export var lighting_and_reflection_on := false
@export var vertex_displacement_on := false
@export var height_map_scale_changed := 0.0
@export var vertex_shader_scale_changed := 0.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#pass
	var light_color = Vector3(directionalLight.light_color.r, directionalLight.light_color.g, directionalLight.light_color.b)
	var light_dir = directionalLight.global_transform.basis.z
	
	var material = terrain.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("directional_light_color", light_color)
		material.set_shader_parameter("directional_light_direction", light_dir)
		material.set_shader_parameter("cool_lighting_on", lighting_and_reflection_on)
		material.set_shader_parameter("vertex_shader_on", vertex_displacement_on)
		material.set_shader_parameter("heightmap_scale", height_map_scale_changed)
		material.set_shader_parameter("vertex_disp_scale", vertex_shader_scale_changed)
