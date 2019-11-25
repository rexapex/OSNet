#pragma once

enum osnetInitError {
	osn_INTERNAL = 1,

	osn_NOT_READY,
	osn_NOT_SUPPORTED,
	osn_IN_PROGRESS
};

// Initialise the Origami Sheep Net library
// Returns CN_SUCCESS (0) on success and a Coblox Net error code on failure
OSN_STATUS osn_init();

// Uninitialise the Origami Sheep Net library
void osn_uninit();
