extends MeshInstance3D

@onready var directional_light = $"../../DirectionalLight3D"
@onready var material_instance = get_active_material(0)
@onready var main_gdscript = $"../.."

@export var glow_color: Color = Color(0.869,0,0.161)
@export var glow_intensity: float = 0.1

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	#pass
	if material_instance is ShinySpikyBallMaterial:
		# Get the light's color.
		var light_color = directional_light.light_color
		# Calculate the effective light direction (negative Z-axis of the light's global transform).
		var light_dir = directional_light.global_transform.basis.z.normalized()
		var lighting_and_reflection_on = main_gdscript.lighting_and_reflection_on
		
		# Update the custom material with the directional light's parameters.
		material_instance.set_directional_light_color(light_color)
		material_instance.set_directional_light_direction(light_dir)
		material_instance.set_glow_color(glow_color)
		material_instance.set_glow_intensity(glow_intensity)
		material_instance.set_lighting_and_reflection(lighting_and_reflection_on)
