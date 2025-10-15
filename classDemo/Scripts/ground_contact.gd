extends MeshInstance3D

@onready var Playerbody = $PlayerBody

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#pass
	var base_height = Playerbody.transform.origin.y + (Playerbody.player_head_height * XRServer.world_scale)
	position = Playerbody.transform.origin
	# Call the function as needed
	var hit_point = cast_downward_ray()
	if hit_point != null:
		var distance = position.distance_to(hit_point)
		# base_height + 0.1 represents if the player is trying to see something with tiptoes. 
		# That means that the player is still on the ground but a little bit taller than their original height.
		if distance <= (base_height + 0.1):
			position.y = hit_point.y
		else:
			position.y = (base_height + 0.1)
	else:
		position.y = (base_height + 0.1)

func cast_downward_ray():
	# Starting point is the global position of your node
	var from = Playerbody.global_transform.origin
	# Cast the ray downward (100 units in this example)
	var to = from - Vector3.UP * 100
	# Get the direct space state for collision queries
	var space_state = get_world_3d().direct_space_state
	var ray_params = PhysicsRayQueryParameters3D.new()
	ray_params.from = from
	ray_params.to = to
	ray_params.exclude = [self]  # Exclude the current node
	
	var result = space_state.intersect_ray(ray_params)
	
	if result:
		var hit_point = result.position  # The collision point
		return hit_point
	else:
		return null
	
