#include "register_types.h"

// Headers for Assignment 1
#include "keyinput.h"
#include "modifyspeed.h"

// Headers for Assignment 2
#include "enhanced_input_handling.h"
#include "coinscontroller.h"
#include "custom_frame_rate_handling.h"

// Headers for Assignment 3
#include "magnetic_constraint.h"
#include "magnetic_constraint_3d.h"
#include "collision_response_controller.h"
#include "custom_platform.h"
#include "wind_2d.h"
#include "custom_gravity_body_2d.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void initialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Register the custom classes for Assignment 1.
	GDREGISTER_CLASS(KeyInput);
	GDREGISTER_CLASS(ModifySpeed);

	// Register the custom classes for Assignment 2.
	GDREGISTER_CLASS(Enhanced_Input_Handling);
	GDREGISTER_CLASS(CoinsController);
	GDREGISTER_CLASS(CustomFrameRateHandling);

	// Register the custom classes for Assignment 3.
	GDREGISTER_CLASS(MagneticConstraint);
	GDREGISTER_CLASS(MagneticConstraint3D);
	GDREGISTER_CLASS(CollisionResponseController);
	GDREGISTER_CLASS(CustomPlatform);
	GDREGISTER_CLASS(WindArea2D);
	GDREGISTER_CLASS(CustomGravityBody2D);
}

void uninitialize_example_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(initialize_example_module);
	init_obj.register_terminator(uninitialize_example_module);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}