extends CharacterBody2D


# Adjust this value to control the force magnitude.
@export var move_speed: float = 100.0
@export var isLeft := false

var _forward_action := "ui_up"
var _backward_action := "ui_down"
var _left_action := "ui_left"
var _right_action := "ui_right"

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var physics_engine = ProjectSettings.get_setting("physics/3d/physics_engine")
	print("Active 3D Physics Engine: ", physics_engine)
	
	if isLeft:
		_forward_action = "move_up_wasd"
		_backward_action = "move_down_wasd"
		_left_action = "move_left_wasd"
		_right_action = "move_right_wasd"
	else:
		_forward_action = "move_up_arrow"
		_backward_action = "move_down_arrow"
		_left_action = "move_left_arrow"
		_right_action = "move_right_arrow"


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	var direction := Vector2.ZERO
	
	# Gather input:
	if Input.is_action_pressed(_forward_action):
		direction.y -= 1
	if Input.is_action_pressed(_backward_action):
		direction.y += 1
	if Input.is_action_pressed(_left_action):
		direction.x -= 1
	if Input.is_action_pressed(_right_action):
		direction.x += 1
	
	# Normalize to ensure consistent force when moving diagonally.
	if direction != Vector2.ZERO:
		direction = direction.normalized()
		velocity = direction * move_speed       # Assign direction * speed to velocity
	move_and_slide()                   # Move the character and handle collisions
