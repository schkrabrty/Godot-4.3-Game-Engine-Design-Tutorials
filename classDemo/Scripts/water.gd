extends Node3D

signal wind_strength_changed(value)
signal wind_speed_changed(value)
signal wind_direction_changed(value)
signal directional_Light_setup(value1, value2)
signal lighting_and_reflection_on(value1, value2)

@export var water_level: float = 0.0         # Y coordinate of the water surface
@export var buoyancy_strength: float = 10.0    # Adjust for stronger or weaker buoyancy # Originally it was 15
@export var wave_push_strength: float = 1.0      # Strength of the horizontal push from wind. # Originally it was 5
@export var height_scale: float = 0.5			# Scale factor to match the shader’s height scaling.

# Variables to store current wind values for physics calculations.
var current_wind_direction: Vector3 = Vector3(1.0, 0.0, 0.0)
var current_wind_speed: float = 1.0
var current_wind_strength: float = 0.3

var total_time: float = 0.0

# FastNoiseLite instance – using FastLite noise for water.
var fast_noise: FastNoiseLite

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#pass # Replace with function body.
	# Initialize FastNoiseLite noise generator.
	fast_noise = FastNoiseLite.new()
	fast_noise.seed = 12345
	fast_noise.frequency = 0.05
	fast_noise.fractal_type = 1
	fast_noise.fractal_octaves = 4
	fast_noise.fractal_lacunarity = 2.0
	fast_noise.fractal_gain = 0.5
	#wind_strength_changed.connect(Callable(self, "_on_wind_strength_changed"))
	#wind_speed_changed.connect(Callable(self, "_on_wind_speed_changed"))
	#wind_direction_changed.connect(Callable(self, "_on_wind_direction_changed"))
	#directional_Light_setup.connect(Callable(self, "_on_directional_Light_changed"))
	#lighting_and_reflection_on.connect(Callable(self, "_on_lighting_and_reflection_on"))

# Replicates the shader’s wave() function using FastNoiseLite.
func wave(position: Vector2) -> float:
	# Sample noise at the given position.
	var noise_val = fast_noise.get_noise_2d(position.x, position.y)
	# Offset the position based on noise.
	position += Vector2(noise_val * 2.0 - 1.0, noise_val * 2.0 - 1.0)
	# Compute wave contribution along X and Y.
	var wv = Vector2(1.0 - abs(sin(position.x)), 1.0 - abs(sin(position.y)))
	return pow(1.0 - pow(wv.x * wv.y, 0.65), 4.0)

# Computes the wave height at a given 2D position and time.
func get_wave_height(pos: Vector2, time: float) -> float:
	# Calculate wind offset.
	var wind_dir = Vector2(current_wind_direction.x, current_wind_direction.z).normalized()
	var wind_offset = wind_dir * current_wind_speed * current_wind_strength * (time * 0.25)
	# Combine several wave samples (matching your shader logic).
	var d = wave((pos + wind_offset) * 0.4) * 0.3
	d += wave((pos - wind_offset) * 0.3) * 0.3
	d += wave((pos + wind_offset) * 0.5) * 0.2
	d += wave((pos - wind_offset) * 0.6) * 0.2
	return d * height_scale

# Approximates the gradient (slope) of the water surface via finite differences.
func get_wave_gradient(pos: Vector2, time: float, eps: float = 0.1) -> Vector2:
	var h1 = get_wave_height(pos + Vector2(eps, 0), time)
	var h2 = get_wave_height(pos - Vector2(eps, 0), time)
	var h3 = get_wave_height(pos + Vector2(0, eps), time)
	var h4 = get_wave_height(pos - Vector2(0, eps), time)
	return Vector2((h1 - h2) / (2 * eps), (h3 - h4) / (2 * eps))

# This function runs in sync with the physics engine.
func _physics_process(delta: float) -> void:
	total_time += delta
	# Get the Area3D node that represents the water's collision region.
	var water_area = $Water/Area3D
	if water_area:
		# Iterate through all physics bodies overlapping the water area.
		for body in water_area.get_overlapping_bodies():
			if body is RigidBody3D:
				# Compute how far below the water surface the body is.
				var body_position = body.global_transform.origin
				# Compute the dynamic water height at the body's XZ position.
				var dynamic_water_height = water_level + get_wave_height(Vector2(body_position.x, body_position.z), total_time)
				var depth = dynamic_water_height - body_position.y
				if depth > 0:
					# Upward buoyant force proportional to depth.
					var buoyant_force = Vector3.UP * buoyancy_strength * depth
					# Get the surface gradient to compute a horizontal force.
					var gradient = get_wave_gradient(Vector2(body_position.x, body_position.z), total_time)
					var gradient_force = Vector3(gradient.x, 0, gradient.y) * wave_push_strength * depth
					# Additional wind force to move the object in the wind direction.
					# You can adjust wind_push_multiplier to taste.
					var wind_push_multiplier = 1.0
					var wind_force = current_wind_direction * current_wind_speed * wind_push_multiplier * depth
					# Combine forces (and apply damping if needed).
					var damping = 0.7
					var total_force = (buoyant_force + gradient_force + wind_force) * damping
					# Apply the combined force to the body.
					body.apply_central_force(total_force)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#pass
	total_time += delta

func _on_wind_strength_changed(value):
	# Update the wind_strength uniform in the shader.
	var material = $Water.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("wind_strength", value)
	current_wind_strength = value

func _on_wind_speed_changed(value):
	# Update the wind_speed uniform in the shader.
	var material = $Water.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("wind_speed", value)
	current_wind_speed = value

func _on_wind_direction_changed(value):
	# Convert slider value (in degrees) to a unit vector for the wind_direction.
	# Here we assume the wind is blowing in the horizontal X-Z plane.
	var angle_radians = value * PI / 180.0
	current_wind_direction = Vector3(cos(angle_radians), 0, sin(angle_radians)).normalized()
	
	var material = $Water.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("wind_direction", current_wind_direction)

func _on_directional_light_setup(value1, value2):
	# Update the wind_speed uniform in the shader.
	var material = $Water.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("directional_light_color", value1)
		material.set_shader_parameter("directional_light_direction", value2)

func _on_lighting_and_reflection_on(value1, value2):
	var material = $Water.get_active_material(0) as ShaderMaterial
	if material:
		material.set_shader_parameter("cool_lighting_on", value1)
		material.set_shader_parameter("reflection_on", value2)
