extends Panel

# Get a reference to your WindArea2D node.
# Adjust the node path according to your scene tree.
@onready var wind_area = $"../WindArea2D"
@onready var wind_direction_slider = $VBoxContainer/HBoxContainer/WindDirectionSlider
@onready var wind_speed_slider = $VBoxContainer/HBoxContainer2/WindSpeedSlider
@onready var gust_intensity_slider = $VBoxContainer/HBoxContainer3/GustIntensitySlider
@onready var gust_frequency_slider = $VBoxContainer/HBoxContainer4/GustFrequencySlider
@onready var air_density_slider = $VBoxContainer/HBoxContainer5/AirDensitySlider
@onready var wind_direction = $"../WindDirection"

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# Set focus mode to NONE so they won't capture keyboard input.
	wind_direction_slider.focus_mode = Control.FOCUS_NONE
	wind_speed_slider.focus_mode = Control.FOCUS_NONE
	gust_intensity_slider.focus_mode = Control.FOCUS_NONE
	gust_frequency_slider.focus_mode = Control.FOCUS_NONE
	air_density_slider.focus_mode = Control.FOCUS_NONE
	
	# Connect the slider signals to functions that update wind parameters.
	wind_direction_slider.value_changed.connect(Callable(self, "_on_wind_direction_changed"))
	wind_speed_slider.value_changed.connect(Callable(self, "_on_wind_speed_changed"))
	gust_intensity_slider.value_changed.connect(Callable(self, "_on_gust_intensity_changed"))
	gust_frequency_slider.value_changed.connect(Callable(self, "_on_gust_frequency_changed"))
	air_density_slider.value_changed.connect(Callable(self, "_on_air_density_changed"))
	
	# Optionally, initialize the slider values based on the current WindArea2D values.
	# Using the getter methods (or property access) to obtain the initial values.
	wind_direction_slider.value = wind_area.get_wind_direction().angle() * 180.0 / PI
	wind_speed_slider.value = wind_area.get_wind_speed()
	gust_intensity_slider.value = wind_area.get_gust_intensity()
	gust_frequency_slider.value = wind_area.get_gust_frequency()
	air_density_slider.value = wind_area.get_air_density()
	wind_direction.position = Vector2(550, 300)


func _on_wind_direction_changed(value):
	# Convert the slider's degree value into radians.
	var angle_radians = value * PI / 180.0
	# Create a normalized vector using the angle.
	var new_direction = Vector2(cos(angle_radians), sin(angle_radians))
	# Set the wind direction using the setter.
	wind_area.set_wind_direction(new_direction)
	wind_direction.rotation_degrees = value


func _on_wind_speed_changed(value):
	wind_area.set_wind_speed(value)


func _on_gust_intensity_changed(value):
	wind_area.set_gust_intensity(value)


func _on_gust_frequency_changed(value):
	wind_area.set_gust_frequency(value)


func _on_air_density_changed(value):
	wind_area.set_air_density(value)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
