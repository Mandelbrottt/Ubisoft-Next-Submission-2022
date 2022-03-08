#pragma once

#include "InputCodes.h"
#include "Math/Vector2.h"

namespace Next::Input
{
	/**
	 * \param a_axis The axis to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return The value of the axis in the range [-1, 1]
	 * \remarks Some axes return [-1, 1] and some return [0, 1]. See \link Axis \endlink for more info.
	 *      <br>If no controller is plugged in, keyboard keys are used to emulate input.
	 *          See NextAPI/AppSettings.h for more info.
	 */
	float
	GetAxis(Axis a_axis, uint8_t a_controller = 0);

	/**
	 * \param a_button The button to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return True if the controller button is currently pressed, false otherwise.
	 * \remark If no controller is plugged in, keyboard keys are used to emulate input.
	 *         See NextAPI/AppSettings.h for more info.
	 */
	bool
	GetButton(GamepadButton a_button, uint8_t a_controller = 0);

	/**
	 * \param a_button The button to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return True if the controller button was pressed this frame, false otherwise.
	 * \remark If no controller is plugged in, keyboard keys are used to emulate input.
	 *         See NextAPI/AppSettings.h for more info.
	 */
	bool
	GetButtonDown(GamepadButton a_button, uint8_t a_controller = 0);

	/**
	 * \param a_button The button to check for input.
	 * \param a_controller The controller to detect input for in the range [0, 3].
	 * \return True if the controller button was released this frame, false otherwise.
	 * \remark If no controller is plugged in, keyboard keys are used to emulate input.
	 *         See NextAPI/AppSettings.h for more info.
	 */
	bool
	GetButtonUp(GamepadButton a_button, uint8_t a_controller = 0);

	/**
	 * \param a_button The button to check for input.
	 * \return True if the mouse button is currently pressed, false otherwise.
	 */
	bool
	GetMouseButton(MouseButton a_button);

	/**
	 * \param a_button The button to check for input.
	 * \return True if the mouse button was pressed this frame, false otherwise.
	 */
	bool
	GetMouseButtonDown(MouseButton a_button);

	/**
	 * \param a_button The mouse button to check for input.
	 * \return True if the mouse button was released this frame, false otherwise.
	 */
	bool
	GetMouseButtonUp(MouseButton a_button);

	/**
	 * \param a_useNormalizedCoords If true, return the mouse position in window coordinates.
	 *                         Otherwise, return the mouse position in normalized device coordinates.
	 * \return The mouse position in the given coordinate type.
	 */
	Vector2
	GetMousePosition(bool a_useNormalizedCoords = false);

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
