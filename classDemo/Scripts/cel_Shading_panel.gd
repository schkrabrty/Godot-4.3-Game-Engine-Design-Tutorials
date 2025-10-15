extends Panel

# Get references to the UI elements
@onready var toggle_button = $VBoxContainer/HBoxContainer/LightandReflectionOnButton
@onready var toggle_button1 = $VBoxContainer/HBoxContainer2/MoveSunButton
# Get reference to the root node's script and directional light node.
# Update the node paths to match your scene tree.
@onready var root_script = $".."
@onready var directional_light = $"../DirectionalLight3D"

# Define colors.
var dawn_color = Color("#ffb347")   # Dawn/dusk color.
var midday_color = Color("#fffaf0")     # Midday (bright) color.

# Animation timing variables.
var cycle_duration := 10.0  # Total cycle duration: 10 seconds (5 seconds forward, 5 seconds reverse)
var time_acc := 0.0         # Accumulated time

var move_sun_activated := false

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#pass # Replace with function body.
	# Set focus mode to NONE so they won't capture keyboard input.
	toggle_button.focus_mode = Control.FOCUS_NONE
	toggle_button1.focus_mode = Control.FOCUS_NONE
	
	# Connect the slider signals to functions that update wind parameters.
	toggle_button.toggled.connect(Callable(self, "_on_toggle_lighting"))
	toggle_button1.toggled.connect(Callable(self, "_on_moving_sun_on"))

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	if (move_sun_activated):
		move_sun(delta)

# This function toggles the lighting variable in the root script.
func _on_toggle_lighting(toggled_on: bool) -> void:
	root_script.lighting_and_reflection_on = toggled_on

func _on_moving_sun_on(toggled_on: bool) -> void:
	move_sun_activated = toggled_on
	
	if (move_sun_activated):
		# Set the initial rotation: (1, 45, 0)
		directional_light.rotation_degrees = Vector3(1, 45, 0)
		time_acc = 0.0
	else:
		directional_light.rotation_degrees = Vector3(-10, -120, 0)
		directional_light.light_color = dawn_color

func move_sun(delta: float) -> void:
	time_acc += delta
	# Use fmod() to compute the current time within the cycle.
	var cycle_time = fmod(time_acc, cycle_duration)
	var half_cycle = cycle_duration / 2.0  # 5 seconds
	
	# Compute normalized t: from 0 to 1 in the first half, then from 1 back to 0 in the second half.
	var t: float = 0.0
	if cycle_time < half_cycle:
		t = cycle_time / half_cycle       # t goes from 0 to 1
	else:
		t = 1.0 - ((cycle_time - half_cycle) / half_cycle)  # t goes from 1 back to 0
	
	# Compute rotation based on t.
	var new_x: float
	var new_y: float
	var new_z: float
	if t <= 0.5:
		# Forward journey: X goes from 1 to -90, Y and Z remain at their initial values.
		new_x = lerp(1.0, -90.0, t * 2.0)
		new_y = 45.0
		new_z = 0.0
	else:
		# Reverse journey: X goes from -90 back to 1, Y and Z switch to the end values.
		new_x = lerp(-90.0, 1.0, (t - 0.5) * 2.0)
		new_y = -135.0
		new_z = -180.0
	
	directional_light.rotation_degrees = Vector3(new_x, new_y, new_z)
	directional_light.light_color = get_sun_color(t)
	# Optionally, update directional_light.energy here if needed.
	
# Interpolates the sun's color.
# At t=0 and t=1 (dawn/dusk), the color is dawn_color.
# At t=0.5 (midday), the color is midday_color.
func get_sun_color(t: float) -> Color:
	if t < 0.5:
		var factor = t * 2.0
		return dawn_color + (midday_color - dawn_color) * factor
	else:
		var factor = (t - 0.5) * 2.0
		return midday_color + (dawn_color - midday_color) * factor
