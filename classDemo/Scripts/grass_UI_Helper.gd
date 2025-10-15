extends Node3D

signal wind_strength_changed(value)
signal wind_speed_changed(value)
signal wind_direction_changed(value)
signal ball_repulsion(value)
signal directional_Light_setup(value1, value2)
signal lighting_and_reflection_on(value1, value2)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass
	#wind_strength_changed.connect(Callable(self, "_on_wind_strength_changed"))
	#wind_speed_changed.connect(Callable(self, "_on_wind_speed_changed"))
	#wind_direction_changed.connect(Callable(self, "_on_wind_direction_changed"))
	#ball_repulsion.connect(Callable(self, "_on_ball_repulsion"))
	#directional_Light_setup.connect(Callable(self, "_on_directional_Light_changed"))
	#lighting_and_reflection_on.connect(Callable(self, "_on_lighting_and_reflection_on"))

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
	#var camera = get_viewport().get_camera_3d()
	#var material = $Grass.get_active_material(0) as ShaderMaterial
	#if camera:
		#material.set_shader_parameter("u_camera_position", camera.global_transform.origin)

func _on_wind_strength_changed(value):
	# Update the wind_strength uniform in the shader.
	var material = $Grass.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("wind_strength", value)

func _on_wind_speed_changed(value):
	# Update the wind_speed uniform in the shader.
	var material = $Grass.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("wind_speed", value)

func _on_wind_direction_changed(value):
	var material = $Grass.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("wind_direction", value)

func _on_ball_repulsion(value):
	var material = $Grass.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("ball_position", value)

func _on_directional_light_setup(value1, value2):
	# Update the wind_speed uniform in the shader.
	var material = $Grass.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("directional_light_color", value1)
		material.set_shader_parameter("directional_light_direction", value2)

func _on_lighting_and_reflection_on(value1, value2):
	var material = $Grass.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("cool_lighting_on", value1)
		material.set_shader_parameter("rim_lighting_on", value2)
