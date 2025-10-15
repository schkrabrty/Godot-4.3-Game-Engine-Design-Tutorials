extends Panel


# Get references to nodes.
@onready var gravity_body = $"../CustomGravityBody2D"
@onready var slider_strength = $VBoxContainer/HBoxContainer/GravityStrengthSlider
@onready var option_mode = $VBoxContainer/HBoxContainer2/GravityModeOption
@onready var slider_center_x = $VBoxContainer/HBoxContainer3/GravityCenterSliderX
@onready var slider_center_y = $VBoxContainer/HBoxContainer4/GravityCenterSliderY
@onready var point = $"../Point"


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# Set focus mode to NONE so they won't capture keyboard input.
	slider_strength.focus_mode = Control.FOCUS_NONE
	option_mode.focus_mode = Control.FOCUS_NONE
	slider_center_x.focus_mode = Control.FOCUS_NONE
	slider_center_y.focus_mode = Control.FOCUS_NONE
	
	# Connect the slider and option button signals to functions that update gravity parameters.
	slider_strength.value_changed.connect(Callable(self, "_on_strength_changed"))
	option_mode.item_selected.connect(Callable(self, "_on_option_mode_item_selected"))
	slider_center_x.value_changed.connect(Callable(self, "_on_slider_center_x_changed"))
	slider_center_y.value_changed.connect(Callable(self, "_on_slider_center_y_changed"))
	
	# Initialize the UI elements based on the current CustomGravityBody2D values.
	slider_strength.value = gravity_body.get_gravity_strength()
	option_mode.selected = gravity_body.get_gravity_mode()
	var value = gravity_body.get_gravity_center()
	slider_center_x.value = value.x
	slider_center_y.value = value.y
	point.position = value


func _on_strength_changed(value):
	# Update the gravity strength on the custom node.
	gravity_body.set_gravity_strength(value)


func _on_option_mode_item_selected(index):
	# Update the gravity mode.
	# The OptionButton's metadata is set to match the enum values.
	gravity_body.set_gravity_mode(index)


func _on_slider_center_x_changed(value):
	# Update the gravity center's X coordinate.
	var center = gravity_body.get_gravity_center()
	center.x = value
	gravity_body.set_gravity_center(center)
	point.position = center


func _on_slider_center_y_changed(value):
	# Update the gravity center's Y coordinate.
	var center = gravity_body.get_gravity_center()
	center.y = value
	gravity_body.set_gravity_center(center)
	point.position = center


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
