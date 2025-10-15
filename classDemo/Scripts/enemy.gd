extends Area2D

@export var speed: float = 100.0
var direction: float = 1.0
var sprite_height: float = 0.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var sprite = $Sprite2D # Consider the Sprite2D node to determine the height of the enemy
	
	if sprite.texture:
		sprite_height = sprite.texture.get_size().y * sprite.scale.y # If pivot/origin is at top-left, we'll use the entire texture height
	else:
		sprite_height = 0.0

# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#print("Delta = ", delta)
	position.y += speed * direction * delta # Update position by speed * direction
	var screen_size: Vector2 = get_viewport_rect().size # Get the screen size (the viewport size)
	
	# If we go beyond the top edge...
	if position.y < 0:
		position.y = 0
		direction = 1.0 # switch to move downward
		
	# If we go beyond the bottom edge...
	if position.y > screen_size.y - sprite_height:
		position.y = screen_size.y - sprite_height
		direction = -1.0 # switch to move upward

func _on_body_entered(body: Node2D) -> void:
	if body is CharacterBody2D and body.name == "Player":
		body.queue_free()  # Destroys the 'body' instead of the area
		var label = $"../../UI Label" as Label # Now show "You died!" message.
		label.text = "You died!"
