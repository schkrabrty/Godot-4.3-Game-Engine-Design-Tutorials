extends Node3D

@export var speed = 40
@export var bullet_scene : PackedScene
@onready var muzzle = $Barrel/Muzzle

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.

# This RPC will be called on all peers.
@rpc("any_peer", "call_local", "unreliable_ordered")
func spawn_bullet(bullet_transform: Transform3D) -> void:
	var bullet = bullet_scene.instantiate()
	# Add the bullet to the current scene so it becomes visible to everyone.
	get_tree().current_scene.add_child(bullet)
	bullet.global_transform = bullet_transform
	# Adjust the bullet scale if needed.
	bullet.scale = Vector3(bullet.scale.x * 6, bullet.scale.y * 6, bullet.scale.z * 2)
	# Use the forward direction from the bullet's transform.
	bullet.linear_velocity = speed * (-bullet_transform.basis.z)
	bullet.start_lifetime(2)
	
# Called every frame. 'delta' is the elapsed time since the previous frame.
func _physics_process(delta: float) -> void:
	if is_multiplayer_authority():
		if Input.is_action_just_pressed("shoot"):
			# Call the RPC so that spawn_bullet is executed on all peers.
			rpc("spawn_bullet", muzzle.global_transform)
