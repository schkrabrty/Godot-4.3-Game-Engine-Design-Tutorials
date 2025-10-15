extends Area2D

# This is a new addition. We can specify how frequently the enemy will move in one second
@export var frameRate: float = 60.0

# This is a new addition. We can specify a speed multiplier if we want to speed up or slow down the enemy 
# This is useful if you want to run the enemy at more than 60 frames per second because Godot's timer can run at 60 frames per second or lower
@export var speedMultiplier: float = 1.0 

var direction: float = 1.0
var sprite_height: float = 0.0

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	var sprite = $Sprite2D # Consider the Sprite2D node to determine the height of the enemy
	
	if sprite.texture:
		sprite_height = sprite.texture.get_size().y * sprite.scale.y # If pivot/origin is at top-left, we'll use the entire texture height
	else:
		sprite_height = 0.0

func _on_body_entered(body: Node2D) -> void:
	if body is CharacterBody2D and body.name == "Player":
		body.queue_free()  # Destroys the 'body' instead of the area
		# Now show "You died!" message. One simple way is to set the text of a Label.
		var label = $"../../UI Label" as Label
		label.text = "You died!"
