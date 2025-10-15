extends RigidBody3D


func start_lifetime(time: float) -> void:
	# Create a one-shot timer and await until it times out
	await get_tree().create_timer(time).timeout
	queue_free()
