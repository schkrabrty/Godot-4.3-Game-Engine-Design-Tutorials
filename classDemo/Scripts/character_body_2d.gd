extends CharacterBody2D


@export var speed := 200.0


func _process(delta: float) -> void:
	var direction := Vector2.ZERO
	
	# Gather input:
	if Input.is_action_pressed("ui_up"):
		direction.y -= 1
	if Input.is_action_pressed("ui_down"):
		direction.y += 1
	if Input.is_action_pressed("ui_left"):
		direction.x -= 1
	if Input.is_action_pressed("ui_right"):
		direction.x += 1
	
	direction = direction.normalized() # Normalize to ensure consistent diagonal speed
	velocity = direction * speed       # Assign direction * speed to velocity
	move_and_slide()                   # Move the character and handle collisions
