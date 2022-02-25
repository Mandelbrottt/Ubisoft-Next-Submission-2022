#pragma once

#include "InputCodes.h"

namespace Next::Input
{
	/**
	 * \param a_axis The axis to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return The value of the axis in the range [-1, 1]
	 * \remarks Some axes return [-1, 1] and some return [0, 1]. See \link Input::Axis \endlink for more info.
	 *      <br>If no controller is plugged in, keyboard keys are used to emulate input.
	 *          See NextAPI/AppSettings.h for more info.
	 */
	float
	GetAxis(AxisCode a_axis, uint8_t a_controller = 0);

	/**
	 * \param a_button The button to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return True if the controller button is currently pressed, false otherwise.
	 * \remark If no controller is plugged in, keyboard keys are used to emulate input.
	 *         See NextAPI/AppSettings.h for more info.
	 */
	bool
	GetButton(ButtonCode a_button, uint8_t a_controller = 0);
	
	/**
	 * \param a_button The button to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return True if the controller button was pressed this frame, false otherwise.
	 * \remark If no controller is plugged in, keyboard keys are used to emulate input.
	 *         See NextAPI/AppSettings.h for more info.
	 */
	bool
	GetButtonDown(ButtonCode a_button, uint8_t a_controller = 0);
	
	/**
	 * \param a_button The button to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return True if the controller button was released this frame, false otherwise.
	 * \remark If no controller is plugged in, keyboard keys are used to emulate input.
	 *         See NextAPI/AppSettings.h for more info.
	 */
	bool
	GetButtonUp(ButtonCode a_button, uint8_t a_controller = 0);
	
	/**
	 * \param a_key The key to check for input.
	 * \return True if the keyboard key is currently pressed, false otherwise.
	 */
	bool
	GetKey(KeyCode a_key);
	
	/**
	 * \param a_key The button to check for input.
	 * \return True if the keyboard key is currently pressed, false otherwise.
	 */
	bool
	GetKeyDown(KeyCode a_key);
	
	/**
	 * \param a_key The button to check for input.
	 * \return True if the keyboard key is currently pressed, false otherwise.
	 */
	bool
	GetKeyUp(KeyCode a_key);
}
