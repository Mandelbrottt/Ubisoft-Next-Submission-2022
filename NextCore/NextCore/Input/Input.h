#pragma once

#include "InputCodes.h"

namespace NextCore::Input
{
	float
	GetAxis(Axis a_axis, uint8_t a_controller = 0);

	bool
	GetButton(Button a_button, uint8_t a_controller = 0);

	bool
	GetButtonDown(Button a_button, uint8_t a_controller = 0);

	bool
	GetButtonUp(Button a_button, uint8_t a_controller = 0);

	bool
	GetKey(Key a_button);

	bool
	GetKeyDown(Key a_key);

	bool
	GetKeyUp(Key a_key);
}
