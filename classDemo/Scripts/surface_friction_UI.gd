extends Panel

# Cache references to UI nodes and your custom node.
@onready var friction_slider = $VBoxContainer/HBoxContainer/FrictionSlider
@onready var bounce_slider = $VBoxContainer/HBoxContainer2/BounceSlider
@onready var rough_check = $VBoxContainer/RoughCheckButton
@onready var absorbent_check = $VBoxContainer/AbsorvantCheckButton
@onready var custom_rigid_body = $"../CustomPlatform"  # Adjust the path as needed


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# Set focus mode to NONE so they won't capture keyboard input.
	friction_slider.focus_mode = Control.FOCUS_NONE
	bounce_slider.focus_mode = Control.FOCUS_NONE
	rough_check.focus_mode = Control.FOCUS_NONE
	absorbent_check.focus_mode = Control.FOCUS_NONE
	#
	## Connect the signals (using the new Godot 4 syntax)
	#friction_slider.value_changed.connect(Callable(custom_rigid_body, "set_material_friction"))
	#bounce_slider.value_changed.connect(Callable(custom_rigid_body, "set_material_bounce"))
	#rough_check.toggled.connect(Callable(custom_rigid_body, "set_rough"))
	#absorbent_check.toggled.connect(Callable(custom_rigid_body, "set_absorbent"))
	
	friction_slider.value_changed.connect(Callable(self, "_on_material_friction_changed"))
	bounce_slider.value_changed.connect(Callable(self, "_on_material_bounce_changed"))
	rough_check.toggled.connect(Callable(self, "_on_rough_changed"))
	absorbent_check.toggled.connect(Callable(self, "_on_absorbent_changed"))
	
	# Optionally, initialize the slider values based on the current WindArea2D values.
	# Using the getter methods (or property access) to obtain the initial values.
	friction_slider.value = custom_rigid_body.get_material_friction()
	bounce_slider.value = custom_rigid_body.get_material_bounce()
	rough_check.set_pressed(custom_rigid_body.get_rough())
	absorbent_check.set_pressed(custom_rigid_body.get_absorbent())


func _on_material_friction_changed(value):
	custom_rigid_body.set_material_friction(value)


func _on_material_bounce_changed(value):
	custom_rigid_body.set_material_bounce(value)


func _on_rough_changed(value):
	custom_rigid_body.set_rough(value)


func _on_absorbent_changed(value):
	custom_rigid_body.set_absorbent(value)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass
