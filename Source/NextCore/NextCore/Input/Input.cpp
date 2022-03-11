#include "pch.h"
#include "Input.h"

#include <bitset>

#include <NextAPI/app.h>

#include "InputConversion.h"

#include "Application/Application.h"

namespace Next::Input
{
	using key_underlying_t = Detail::InputMap::key_underlying_t;
	using gamepad_button_underlying_t = Detail::InputMap::gamepad_button_underlying_t;
	using mouse_button_underlying_t = Detail::InputMap::mouse_button_underlying_t;

	constexpr int NUM_KEYS          = Detail::InputMap::key_size;
	constexpr int NUM_MOUSE_BUTTONS = Detail::InputMap::mouse_button_size;

	constexpr static Detail::InputMap g_inputMap;

	/* TODO: We don't need this many keys, but we dont need to do additional processing
	         to determine the largest value in the keys enum */
	static std::bitset<NUM_KEYS> g_thisFrameKeys { false };
	static std::bitset<NUM_KEYS> g_lastFrameKeys { false };

	static std::bitset<NUM_MOUSE_BUTTONS> g_thisFrameMouseButtons { false };
	static std::bitset<NUM_MOUSE_BUTTONS> g_lastFrameMouseButtons { false };

	static Detail::InputMap::gamepad_button_underlying_t g_thisFrameButtons[MAX_CONTROLLERS] { 0 };
	static Detail::InputMap::gamepad_button_underlying_t g_lastFrameButtons[MAX_CONTROLLERS] { 0 };

	void
	Update()
	{
		// Move last frames keys to the last frame array
		g_lastFrameKeys = g_thisFrameKeys;

		for (int key = 0; key < g_thisFrameKeys.size(); key++)
		{
			g_thisFrameKeys[key] = App::IsKeyPressed(key);
		}

		// Move last frames mouse buttons to the last frame array
		g_lastFrameMouseButtons = g_thisFrameMouseButtons;

		for (int mouseButton = 0; mouseButton < g_thisFrameMouseButtons.size(); mouseButton++)
		{
			// NOTE: Currently App::IsKeyPressed is just a wrapper around GetAsyncKeyState
			//       Windows implements mouse buttons as virtual keys, so we get their state with
			//       with the same function as keys for now.
			g_thisFrameMouseButtons[mouseButton] = App::IsKeyPressed(mouseButton);
		}

		std::memcpy(g_lastFrameButtons, g_thisFrameButtons, sizeof(g_lastFrameButtons));
		std::memset(g_thisFrameButtons, 0, sizeof(g_thisFrameButtons));

		for (int i = 0; i < MAX_CONTROLLERS; i++)
		{
			auto& controller = App::GetController(i);
			for (gamepad_button_underlying_t button = 1; button < 1 << 15; button <<= 1)
			{
				g_thisFrameButtons[i] |= controller.CheckButton(button, false) ? button : 0;
			}
		}
	}

	float
	GetAxis(Axis a_axis, uint8_t a_controller)
	{
		auto& controller = App::GetController(a_controller);

		// Keep outside switch to use across multiple cases
		float result;
		int   emulated_key;

		switch (a_axis)
		{
			case Axis::LeftStickX:
				return controller.GetLeftThumbStickX();
			case Axis::LeftStickY:
				// BUG: Emulated keys return the wrong-signed value for StickY, so we adjust for that here
				result = controller.GetLeftThumbStickY();
				emulated_key = result < 0
					               ? APP_PAD_EMUL_LEFT_THUMB_UP
					               : APP_PAD_EMUL_LEFT_THUMB_DOWN;
				return g_thisFrameKeys[emulated_key] ? -result : result;
			case Axis::RightStickX:
				return controller.GetRightThumbStickX();
			case Axis::RightStickY:
				result = controller.GetRightThumbStickY();
				emulated_key = result < 0
					               ? APP_PAD_EMUL_RIGHT_THUMB_UP
					               : APP_PAD_EMUL_RIGHT_THUMB_DOWN;
				return g_thisFrameKeys[emulated_key] ? -result : result;
			case Axis::LeftTrigger:
				return controller.GetLeftTrigger();
			case Axis::RightTrigger:
				return controller.GetRightTrigger();
			default:
				throw "Invalid Axis";
		}
	}

	bool
	GetButton(GamepadButton a_button, uint8_t a_controller)
	{
		gamepad_button_underlying_t thisButton = g_thisFrameButtons[a_controller];

		gamepad_button_underlying_t buttonToCheck = g_inputMap[a_button];

		return (thisButton & buttonToCheck);
	}

	bool
	GetButtonDown(GamepadButton a_button, uint8_t a_controller)
	{
		gamepad_button_underlying_t thisButton = g_thisFrameButtons[a_controller];
		gamepad_button_underlying_t lastButton = g_lastFrameButtons[a_controller];

		gamepad_button_underlying_t buttonToCheck = g_inputMap[a_button];

		return (thisButton & buttonToCheck) && !(lastButton & buttonToCheck);
	}

	bool
	GetButtonUp(GamepadButton a_button, uint8_t a_controller)
	{
		gamepad_button_underlying_t thisButton = g_thisFrameButtons[a_controller];
		gamepad_button_underlying_t lastButton = g_lastFrameButtons[a_controller];

		gamepad_button_underlying_t buttonToCheck = g_inputMap[a_button];

		return !(thisButton & buttonToCheck) && (lastButton & buttonToCheck);
	}

	bool
	GetMouseButton(MouseButton a_button)
	{
		mouse_button_underlying_t mouseButton = g_inputMap[a_button];

		return g_thisFrameMouseButtons[mouseButton];
	}

	bool
	GetMouseButtonDown(MouseButton a_button)
	{
		mouse_button_underlying_t mouseButton = g_inputMap[a_button];

		return g_thisFrameMouseButtons[mouseButton] && !g_lastFrameMouseButtons[mouseButton];
	}

	bool
	GetMouseButtonUp(MouseButton a_button)
	{
		mouse_button_underlying_t mouseButton = g_inputMap[a_button];

		return !g_thisFrameMouseButtons[mouseButton] && g_lastFrameMouseButtons[mouseButton];
	}

	Vector2
	GetMousePosition(bool a_useNormalizedCoords)
	{
		Vector2 result;
		App::GetMousePos(result.x, result.y);

		result = result * 0.5f + Vector2(0.5f);

		if (a_useNormalizedCoords == false)
		{
			result.x *= Application::ScreenWidth();
			result.y *= Application::ScreenHeight();
		}
		
		return result;
	}

	bool
	GetKey(KeyCode a_key)
	{
		key_underlying_t key = g_inputMap[a_key];

		return g_thisFrameKeys[key];
	}

	bool
	GetKeyDown(KeyCode a_key)
	{
		key_underlying_t key = g_inputMap[a_key];

		return g_thisFrameKeys[key] && !g_lastFrameKeys[key];
	}

	bool
	GetKeyUp(KeyCode a_key)
	{
		key_underlying_t key = g_inputMap[a_key];

		return !g_thisFrameKeys[key] && g_lastFrameKeys[key];
	}
}
