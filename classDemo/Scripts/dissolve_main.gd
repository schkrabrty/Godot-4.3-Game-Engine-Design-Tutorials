extends Node3D

@onready var directionalLight := $DirectionalLight3D
@onready var dissolvingBall := $DissolvingBall

@export var lighting_and_reflection_on := false
@export var explosion_on := false
@export var explosion_speed_changed := 0.0
@export var dissolve_delay_changed := 0.0
@export var dissolve_speed_changed := 0.0
@export var explosion_time := 0.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#pass
	# Update explosion_time if the explosion is active
	if explosion_on:
		explosion_time += delta
	
	var light_color = Vector3(directionalLight.light_color.r, directionalLight.light_color.g, directionalLight.light_color.b)
	var light_dir = directionalLight.global_transform.basis.z
	
	var material = dissolvingBall.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("directional_light_color", light_color)
		material.set_shader_parameter("directional_light_direction", light_dir)
		material.set_shader_parameter("cool_lighting_on", lighting_and_reflection_on)
		material.set_shader_parameter("explosion_on", explosion_on)
		material.set_shader_parameter("explosion_speed", explosion_speed_changed)
		material.set_shader_parameter("dissolve_delay", dissolve_delay_changed)
		material.set_shader_parameter("dissolve_speed", dissolve_speed_changed)
		material.set_shader_parameter("explosion_time", explosion_time)
