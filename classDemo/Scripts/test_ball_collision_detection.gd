extends RigidBody3D

@export var impact_threshold := 2.0  # Minimum impulse to trigger a dent.
var current_dent_strength := 0.0  # Track the current dent strength.

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	#pass # Replace with function body.
	set_contact_monitor(true)
	set_max_contacts_reported(10000000);


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass


func _integrate_forces(state: PhysicsDirectBodyState3D) -> void:
	var contact_count = state.get_contact_count()
	for i in range(contact_count):
		var impulse = state.get_contact_impulse(i)
		if impulse.length() > impact_threshold:
			var contact_point = state.get_contact_local_position(i)
			# print("Global: ", contact_point, " Local: ", to_local(contact_point))
			# Access the MeshInstance3D that holds the shader material.
			var material = $MeshInstance3D.get_active_material(0) as ShinySpikyBallMaterial
			var new_strength = clamp(impulse.length() * 0.1, 0.0, 1.0)  # Increased multiplier to 0.1
			current_dent_strength = new_strength  # Store the value for decay.
			material.set_dent_center(to_local(contact_point))
			material.set_dent_strength(current_dent_strength)
			# print("Impulse:", impulse.length(), " -> Dent Strength:", current_dent_strength)
			break  # Only handle one contact per frame.

func _physics_process(delta: float) -> void:
	# Gradually decay the dent strength over time.
	if current_dent_strength > 0.0:
		current_dent_strength = max(0.0, current_dent_strength - delta * 0.05)
		var material = $MeshInstance3D.get_active_material(0) as ShinySpikyBallMaterial
		material.set_dent_strength(current_dent_strength)
