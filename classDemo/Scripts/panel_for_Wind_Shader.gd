extends Node3D

var GrassScene = preload("res://Scenes/grass.tscn")
var RockScene = preload("res://Scenes/rock.tscn")
@onready var WaterScene = $WaterBody
var rock_container: Node3D

## -----> Use this line if you do not want to use MeshInstance3D <-----
var grass_container: Node3D

var time_accumulator := 0.0

# Wind parameters (for grasses)
var base_strength := 0.3
var amplitude_strength := 0.1
var base_speed := 1.0
var amplitude_speed := 0.2

# Terrain parameters (must match your shader)
var height_scale := 0.2
var slope_factor := 2.0
var slope_start := -8.0
var slope_end := 8.0

# Noise generator instance (using FastNoiseLite)
var noise

@onready var terrain := $Terrain
@export var terrain_size_x := 16.0 # Default, in case size cannot be retrieved
@export var terrain_size_z := 16.0
@export var number_of_grasses := 5000
@export var height_offset := 0.1
@export var lighting_and_reflection_on := false
@export var rim_lighting_on := false
@export var reflection_on := false
var original_terrain_size := terrain_size_x
var original_water_size := terrain_size_x
@onready var ball = $TestBall
@onready var directionalLight := $DirectionalLight3D
@onready var reflectionProbe := $ReflectionProbe

# Define fixed rock positions and rotations.
# For example, here are 6 positions and rotations.
# Adjust these vectors to your desired coordinates/angles.
var rock_positions = [
	Vector3(-7.2, 0, -3.5),
	Vector3( 5.0, 0, -6.2),
	Vector3(-2.3, 0,  6.5),
	Vector3( 4.6, 0,  4.2),
	Vector3(-5.8, 0,  2.9),
	Vector3(-3.4, 0, -6.5)
	#Vector3(20.0, 0, -5.0),
	#Vector3(-18.0, 0,  6.0),
	#Vector3(10.0, 0,  6.5),
	#Vector3(-15.0, 0, -3.0),
	#Vector3(8.0,  0, -1.0),
	#Vector3(18.0, 0,  3.0)
]
# Rotations in degrees (converted to radians later) for each rock.
var rock_rotations = [
	Vector3(0, 27, 0),
	Vector3(0, 113, 0),
	Vector3(0, 68, 0),
	Vector3(0, 245, 0),
	Vector3(0, 152, 0),
	Vector3(0, 310, 0)
	#Vector3(0, 45, 0),
	#Vector3(0, 90, 0),
	#Vector3(0, 135, 0),
	#Vector3(0, 200, 0),
	#Vector3(0, 275, 0),
	#Vector3(0, 350, 0)
]
# Sample new base color values for each rock (you could also randomize these).
var new_base_colors = [
	Vector3(0.1, 0.1, 0.1),  # black
	Vector3(0.1, 0.1, 0.1),
	Vector3(0.1, 0.1, 0.1),
	Vector3(0.1, 0.1, 0.1),
	Vector3(0.1, 0.1, 0.1),
	Vector3(0.1, 0.1, 0.1)
	#Vector3(0.1, 0.1, 0.1),
	#Vector3(0.1, 0.1, 0.1),
	#Vector3(0.1, 0.1, 0.1),
	#Vector3(0.1, 0.1, 0.1),
	#Vector3(0.1, 0.1, 0.1),
	#Vector3(0.1, 0.1, 0.1)
]
# Sample new top color values for each rock (you could also randomize these).
var new_top_colors = [
	Vector3(0.8, 0.6, 0.35),  # A brighter, warmer tone
	Vector3(0.9, 0.55, 0.30),  # Slightly less saturated
	Vector3(1.0, 0.65, 0.40),  # Even brighter with more saturation
	Vector3(1.1, 0.50, 0.28),  # A subtle variant for contrast
	Vector3(1.2, 0.70, 0.45),  # The most pronounced, creating a distinct top
	Vector3(1.3, 0.60, 0.38)   # Similar to the first, with minor variation
	#Vector3(1.4, 0.50, 0.30),
	#Vector3(1.5, 0.75, 0.50),
	#Vector3(1.6, 0.60, 0.33),
	#Vector3(1.7, 0.68, 0.42),
	#Vector3(1.8, 0.55, 0.35),
	#Vector3(1.9, 0.72, 0.47)
]
# Sample new scale values for each rock (you could also randomize these).
var new_scales = [
	Vector3(1.0, 1.8, 0.8),  # A rock that's much taller than wide or deep
	Vector3(0.7, 1.9, 1.4),  # Very tall and slightly elongated in depth
	Vector3(1.6, 0.9, 1.3),  # Wider on X, but flatter vertically
	Vector3(0.6, 1.2, 1.8),  # Compressed on X, but extended in depth
	Vector3(1.8, 1.4, 0.7),  # Stretched on X and tall, but shallow in Z
	Vector3(1.2, 0.8, 2.0)   # Very elongated in depth with reduced height
	#Vector3(1.5, 2.0, 1.0),
	#Vector3(0.9, 1.7, 1.6),
	#Vector3(1.4, 1.5, 1.2),
	#Vector3(0.8, 1.6, 1.9),
	#Vector3(1.7, 1.3, 1.0),
	#Vector3(1.3, 1.9, 1.4)
]

# Exclusion radius: grass will not be placed within this radius of a rock.
var rock_exclusion_radius := 1.6

var count := 0

## -----> Use these two lines if you want to use MultiMesh3D <-----
## We'll store the MultiMeshInstance3D in a member variable.
#var grass_multimesh_instance : MultiMeshInstance3D

func _ready():
	var water_mesh := WaterScene.get_node("Water")
	
	if terrain and terrain is MeshInstance3D:
		var scale_x = terrain_size_x / original_terrain_size
		var scale_z = terrain_size_z / original_terrain_size
		terrain.scale = Vector3(scale_x, terrain.scale.y, scale_z)
	
	if water_mesh and water_mesh is MeshInstance3D:
		var scale_x = terrain_size_x / original_water_size
		var scale_z = terrain_size_z / original_water_size
		# Set the scale while preserving the current Y scale.
		water_mesh.scale = Vector3(scale_x, water_mesh.scale.y, scale_z)
	
	# Initialize the noise generator.
	noise = FastNoiseLite.new()
	noise.seed = 12345
	noise.frequency = 0.05
	noise.fractal_type = 1
	noise.fractal_octaves = 4
	noise.fractal_lacunarity = 2.0
	noise.fractal_gain = 0.5
	
	rock_container = Node3D.new()
	rock_container.name = "RockContainer"
	add_child(rock_container)
	
	## -----> Use these four lines if you do not want to use MultiMesh3D <-----
	# Create containers for grass and rocks.
	grass_container = Node3D.new()
	grass_container.name = "GrassContainer"
	add_child(grass_container)
	
	# Now update the borders based on the new terrain_size_x.
	update_borders()
	# After the terrain is properly set up, call the function to generate the collision shape:
	generate_terrain_collision()
	
	# Instantiate rocks at fixed positions and with fixed rotations.
	for i in range(rock_positions.size()):
		var rock_instance = RockScene.instantiate()
		var rock_pos = rock_positions[i]
		
		# Compute the correct y value based on the terrain height.
		rock_pos.y = compute_height(rock_pos.x, rock_pos.z)
		rock_instance.transform.origin = rock_pos
		
		# Apply rotation (convert from degrees to radians)
		rock_instance.rotation = rock_rotations[i] * PI / 180.0
		rock_container.add_child(rock_instance)
	
	for instance in rock_container.get_children():
		instance.emit_signal("update_parameters", new_base_colors[count], new_top_colors[count], new_scales[count])
		count += 1
	
	## -----> Use these twenty lines if you do not want to use MultiMesh3D <-----
	# Instantiate grass across the terrain.
	# Terrain spans 16m x 16m (x and z from -8 to 8).
	for i in range(number_of_grasses):
		var x = randf() * terrain_size_x - (terrain_size_x / 2)
		var z = randf() * terrain_size_z - (terrain_size_z / 2)
		var pos = Vector3(x, compute_height(x, z), z)
		
		# Check if this position is too close to any rock.
		var too_close = false
		for rock_pos in rock_positions:
			if pos.distance_to(rock_pos) < rock_exclusion_radius:
				too_close = true
				break
		if too_close:
			continue  # Skip spawning grass here.
		
		# Instantiate the grass and set its position.
		var grass_instance = GrassScene.instantiate()
		grass_instance.transform.origin = pos
		grass_container.add_child(grass_instance)
	
	## -----> Use these fifty two lines if you want to use MultiMesh3D <-----
	## Create a new MultiMeshInstance3D.
	#grass_multimesh_instance = MultiMeshInstance3D.new()
	#var mm = MultiMesh.new()
	#mm.transform_format = MultiMesh.TRANSFORM_3D
	#grass_multimesh_instance.multimesh = mm
	#grass_multimesh_instance.material_override = preload("res://Materials/grass.tres")
	#
	## Retrieve the mesh from an instance of your GrassScene.
	#var grass_inst = GrassScene.instantiate()
	#var grass_mesh = null
	## If your scene is a MeshInstance3D:
	#if grass_inst is MeshInstance3D:
		#grass_mesh = grass_inst.mesh
	#else:
		## Or try to get a MeshInstance3D child.
		#grass_mesh = grass_inst.get_node("Grass").mesh
		#
	#if grass_mesh == null:
		#push_error("Grass mesh not found! Check your grass scene setup.")
		#return
	#
	#mm.mesh = grass_mesh
	#mm.instance_count = number_of_grasses
	#
	## Fill the MultiMesh with instance transforms.
	#var valid_index = 0
	#for i in range(number_of_grasses):
		#var x = randf() * terrain_size_x - (terrain_size_x / 2.0)
		#var z = randf() * terrain_size_z - (terrain_size_z / 2.0)
		#var pos = Vector3(x, compute_height(x, z), z)
		#
		## Skip positions that are too close to any rock.
		#var too_close = false
		#for rock_pos in rock_positions:
			#if pos.distance_to(rock_pos) < rock_exclusion_radius:
				#too_close = true
				#break
		#if too_close:
			#continue  # Skip this position.
		#
		## Set the transform for this instance.
		#var t = Transform3D.IDENTITY
		#t.origin = pos
		#mm.set_instance_transform(valid_index, t)
		#valid_index += 1
		#shader_caller(0.0)
		#
		## If we've placed as many as we need, break.
		#if valid_index >= number_of_grasses:
			#break
	#
	#add_child(grass_multimesh_instance)

func compute_height(x: float, z: float) -> float:
	# Convert world (x, z) to local coordinates for texture mapping.
	var local_x = x / (terrain_size_x / 2.0)
	var local_z = z / (terrain_size_z / 2.0)
	# Map these to [0,1] as required by the shader's logic.
	var tex_coord = Vector2(local_x, local_z) / 2.0 + Vector2(0.5, 0.5)
	
	# Sample the noise using the same scaling factor as your shader.
	var noise_val = noise.get_noise_2d(tex_coord.x * 10.0, tex_coord.y * 10.0)
	noise_val = (noise_val + 1.0) / 2.0  # Remap noise from [-1,1] to [0,1]
	
	var base_height = noise_val * height_scale
	
	# Apply the slope adjustment using smoothstep.
	var curved_slope = smoothstep(slope_start, slope_end, z)
	var slope_adjustment = curved_slope * slope_factor
	
	## -----> Use these two lines if you do not want to use MultiMesh3D <-----
	# Add the offset to ensure positive height.
	return base_height - slope_adjustment - height_offset
	
	### -----> Use these two lines if you want to use MultiMesh3D <-----
	## Add the offset to ensure positive height.
	#return base_height - slope_adjustment + height_offset

# Add the collision generation function here:
func generate_terrain_collision():
	var grid_size = 200  # Increased resolution to capture more detail
	var step_x = terrain_size_x / (grid_size - 1)
	var step_z = terrain_size_z / (grid_size - 1)
	
	var st = SurfaceTool.new()
	st.begin(Mesh.PRIMITIVE_TRIANGLES)
	
	var vertices = []
	for i in range(grid_size):
		vertices.append([])
		for j in range(grid_size):
			var x = -terrain_size_x / 2.0 + i * step_x
			var z = -terrain_size_z / 2.0 + j * step_z
			var y = compute_height(x, z)
			vertices[i].append(Vector3(x, y, z))
	
	# Create two triangles for each grid cell
	for i in range(grid_size - 1):
		for j in range(grid_size - 1):
			var a = vertices[i][j]
			var b = vertices[i+1][j]
			var c = vertices[i][j+1]
			var d = vertices[i+1][j+1]
			
			st.add_vertex(a)
			st.add_vertex(b)
			st.add_vertex(c)
			
			st.add_vertex(c)
			st.add_vertex(b)
			st.add_vertex(d)
	
	var collision_mesh = st.commit()
	var shape = collision_mesh.create_trimesh_shape()
	
	var static_body = StaticBody3D.new()
	static_body.name = "TerrainCollision"
	var collision_shape = CollisionShape3D.new()
	collision_shape.shape = shape
	static_body.add_child(collision_shape)
	
	# Adjust collision shape's scale to counteract terrain scaling in X and Z.
	var scale_x = terrain.scale.x
	var scale_z = terrain.scale.z
	static_body.scale = Vector3(1.0 / scale_x, 1.0, 1.0 / scale_z)
	
	terrain.add_child(static_body)

func update_borders():
	# Get the Borders node (make sure this node exists as a child of your main node)
	var borders = $Borders
	if borders:
		# Update the side borders' positions.
		# Border_Right: move to (terrain_size_x/2, 1, 7.826)
		var border_left = $Borders/LeftBorder
		if border_left:
			border_left.global_transform.origin = Vector3(terrain_size_x / 2.0, 1, 7.826)
			
		# Border_Left: move to (-terrain_size_x/2, 1, 7.826)
		var border_right = $Borders/RightBorder
		if border_right:
			border_right.global_transform.origin = Vector3(-terrain_size_x / 2.0, 1, 7.826)
		
		# Calculate a scaling factor relative to the original terrain size.
		var scale_factor = terrain_size_x / 16  # assuming 16 is your original full width.
		
		# Update the top border (FrontBorder) scale.
		var border_top = $Borders/FrontBorder
		if border_top:
			border_top.scale.x = scale_factor
			# Optionally, you can adjust its position if needed.
			
		# Update the bottom border (BackBorder) scale.
		var border_bottom = $Borders/BackBorder
		if border_bottom:
			border_bottom.scale.x = scale_factor
			# Optionally, adjust its position if necessary.
		
		var reflectionprobeSize = reflectionProbe.size
		reflectionProbe.size = Vector3(terrain_size_x, reflectionprobeSize.y, reflectionprobeSize.z)

func _process(delta: float) -> void:
	shader_caller(delta)

func shader_caller(delta: float):
	time_accumulator += delta
	
	# Instead of modulating with envelope and sine (which can hit zero),
	# we simply add a constant amplitude so the wind is always nonzero.
	var wind_strength = base_strength + amplitude_strength
	var wind_speed = base_speed + amplitude_speed
	
	# Use a continuously rotating angle for wind direction.
	# Here, the wind rotates at 0.2 radians per second.
	var wind_angle_radians = time_accumulator * 0.4
	# Convert to degrees (if your grass shader expects degrees)
	var wind_angle_degrees = fposmod((wind_angle_radians * 180.0) / PI, 360.0)
	
	# Convert the angle into a 2D directional vector on the XZ plane.
	var wind_direction = Vector3(cos(wind_angle_radians), 0.0, sin(wind_angle_radians))
	
	# Get current ball position and light parameters.
	var ball_pos = ball.global_transform.origin
	var light_color = Vector3(directionalLight.light_color.r, directionalLight.light_color.g, directionalLight.light_color.b)
	var light_dir = directionalLight.global_transform.basis.z
	
	## -----> Use these ten lines if you want to use MultiMesh3D <-----
	## Update the shader uniforms on your material override.
	## (This assumes your material has parameters "wind_strength", "wind_speed", and "wind_direction".)
	#if grass_multimesh_instance.material_override:
		#grass_multimesh_instance.material_override.set_shader_parameter("wind_strength", wind_strength)
		#grass_multimesh_instance.material_override.set_shader_parameter("wind_speed", wind_speed)
		#grass_multimesh_instance.material_override.set_shader_parameter("wind_direction", wind_direction)
		#grass_multimesh_instance.material_override.set_shader_parameter("ball_position", ball_pos)
		#grass_multimesh_instance.material_override.set_shader_parameter("directional_light_color", light_color)
		#grass_multimesh_instance.material_override.set_shader_parameter("directional_light_direction", light_dir)
		#grass_multimesh_instance.material_override.set_shader_parameter("lighting_and_reflection_on", lighting_and_reflection_on)
	
	## -----> Use these ten lines if you do not want to use MultiMesh3D <-----
	# Update each grass instance with the new wind parameters.
	for instance in grass_container.get_children():
		instance.emit_signal("wind_strength_changed", wind_strength)
		instance.emit_signal("wind_speed_changed", wind_speed)
		instance.emit_signal("wind_direction_changed", wind_direction)
		instance.emit_signal("ball_repulsion", ball_pos)
		instance.emit_signal("directional_Light_setup", light_color, light_dir)
		instance.emit_signal("lighting_and_reflection_on", lighting_and_reflection_on, rim_lighting_on)
	
	# Optionally, if you prefer to work with a 2D vector, you could compute:
	# var wind_vector = Vector2(cos(wind_angle_radians), sin(wind_angle_radians))
	# and then update your grass/water scripts accordingly.
	
	# Update the water scene with the new wind parameters.
	WaterScene.emit_signal("wind_strength_changed", wind_strength)
	WaterScene.emit_signal("wind_speed_changed", wind_speed)
	WaterScene.emit_signal("wind_direction_changed", wind_angle_degrees)
	WaterScene.emit_signal("directional_Light_setup", light_color, light_dir)
	WaterScene.emit_signal("lighting_and_reflection_on", lighting_and_reflection_on, reflection_on)
	
	for instance in rock_container.get_children():
		instance.emit_signal("light_parameters", light_dir, light_color)
		instance.emit_signal("lighting_and_reflection_on", lighting_and_reflection_on)
	
	var material = terrain.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("directional_light_color", light_color)
		material.set_shader_parameter("directional_light_direction", light_dir)
		material.set_shader_parameter("cool_lighting_on", lighting_and_reflection_on)
