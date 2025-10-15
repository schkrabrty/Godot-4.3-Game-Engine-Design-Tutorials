extends Node3D

# Define a custom signal that passes new values for the parameters.
signal update_parameters(new_base_color, new_top_color, new_scale)
signal light_parameters(new_light_direction, new_light_color)
signal lighting_and_reflection_on(value)

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#pass
	# Create collision shape when the rock enters the scene.
	add_collision_shape()
	# Connect the signal to our update function.
	#update_parameters.connect(Callable(self, "_on_update_parameters"))
	#light_parameters.connect(Callable(self, "_on_light_parameters"))
	#lighting_and_reflection_on.connect(Callable(self, "_on_lighting_and_reflection_on"))

func add_collision_shape():
	# Assume that your visual mesh is in a child node named "Rock"
	var rock_mesh_instance = $Rock
	if not rock_mesh_instance:
		print("Error: No 'Rock' node found!")
		return
	
	var mesh = rock_mesh_instance.mesh
	if mesh:
		# Create a collision shape from the mesh.
		# Optionally, use create_convex_shape() for an outer-boundary collision shape.
		var collision_shape_resource = mesh.create_convex_shape()
		var collision_shape = CollisionShape3D.new()
		collision_shape.shape = collision_shape_resource
		
		# Create a StaticBody3D to hold the collision shape.
		var static_body = StaticBody3D.new()
		static_body.name = "RockStaticBody"
		static_body.add_child(collision_shape)
		
		# Optionally, reset the collision shape's transform if necessary.
		collision_shape.transform = Transform3D.IDENTITY
		
		# Adjust the collision shape's scale to counteract the rock's scale.
		# This works like in the terrain example.
		var rock_scale = rock_mesh_instance.scale
		# The original scale of the generated collision shape was too tiny. 
		# So, we can increase the scale of the StaticBody by 3 times.
		static_body.scale = Vector3((1.0 / rock_scale.x) * 3.0, (1.0 / rock_scale.y) * 3.0, (1.0 / rock_scale.z) * 3.0) 
		
		# Add the static body as a child of the Rock node.
		rock_mesh_instance.add_child(static_body)
	else:
		print("Error: No mesh found in 'Rock' node.")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _on_update_parameters(new_base_color, new_top_color, new_scale):
	var material = $Rock.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("axis_scale", new_scale)
		material.set_shader_parameter("base_color", new_base_color)
		material.set_shader_parameter("top_color", new_top_color)

func _on_light_parameters(new_light_direction, new_light_color):
	var material = $Rock.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("directional_light_direction", new_light_direction)
		material.set_shader_parameter("directional_light_color", new_light_color)

func _on_lighting_and_reflection_on(value):
	var material = $Rock.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("cool_lighting_on", value)
