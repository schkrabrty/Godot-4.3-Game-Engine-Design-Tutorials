extends Camera3D

@export var target: NodePath
@export var distance: float = 2.0   # Distance behind the ball # Originally it was 10.0
@export var height: float = 2.0      # Height offset from the ball # Originlly it was 5.0
@export var smooth_speed: float = 5.0

var target_node: Node3D

# Stores the last known movement direction; initialize to a default value.
var last_direction: Vector3 = -Vector3.FORWARD

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	# pass # Replace with function body.
	target_node = get_node(target) as Node3D


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	# pass
	if target_node:
		var target_pos = target_node.global_transform.origin
		
		# Get the ball's velocity and ignore the vertical component.
		var velocity = target_node.linear_velocity
		velocity.y = 0  # Ignore any vertical movement.
		
		# If the horizontal velocity is significant, update last_direction.
		if velocity.length() > 0.1:
			last_direction = velocity.normalized()
		
		# Calculate the desired position behind the ball based on its horizontal movement.
		var desired_position = target_pos - last_direction * distance
		desired_position.y += height
		
		# Smoothly interpolate the camera's position.
		global_transform.origin = global_transform.origin.lerp(desired_position, smooth_speed * delta)
		
		# Ensure the camera always looks at the ball, with a fixed up vector.
		look_at(target_pos, Vector3.UP)
