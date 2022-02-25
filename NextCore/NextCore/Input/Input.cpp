#include "pch.h"
#include "Input.h"

#include <bitset>

#include <NextAPI/app.h>

#include "InputConversion.h"

namespace Next::Input
{
	constexpr static Detail::InputMap g_inputMap;
	
	using key_underlying_t = std::underlying_type_t<KeyCode>;
	using button_underlying_t = std::underlying_type_t<ButtonCode>;

	/* TODO: We don't need this many keys, but we dont need to do additional processing
	         to determine the largest value in the keys enum */
	static std::bitset<256> g_thisFrameKeys { false };
	static std::bitset<256> g_lastFrameKeys { false };

	static button_underlying_t g_thisFrameButtons[MAX_CONTROLLERS] { 0 };
	static button_underlying_t g_lastFrameButtons[MAX_CONTROLLERS] { 0 };

	void
	Update()
	{
		// Move last frames keys to the last frame array
		g_lastFrameKeys = g_thisFrameKeys;

		for (int key = 0; key < g_thisFrameKeys.size(); key++)
		{
			g_thisFrameKeys[key] = App::IsKeyPressed(key);
		}
		
		std::memcpy(g_lastFrameButtons, g_thisFrameButtons, sizeof(g_lastFrameButtons));
		std::memset(g_thisFrameButtons, 0, sizeof(g_thisFrameButtons));

		for (int i = 0; i < MAX_CONTROLLERS; i++)
		{
			auto& controller = App::GetController(i);
			for (button_underlying_t button = 1; button < 1 << 15; button <<= 1)
			{
				g_thisFrameButtons[i] |= controller.CheckButton(button, false) ? button : 0;
			}
		}
	}
	
	float
	GetAxis(AxisCode a_axis, uint8_t a_controller)
	{
		auto& controller = App::GetController(a_controller);

		// Keep outside switch to use across multiple cases
		float result;
		int   emulated_key;

		switch (a_axis)
		{
			case AxisCode::LeftStickX:
				return controller.GetLeftThumbStickX();
			case AxisCode::LeftStickY:
				// BUG: Emulated keys return the wrong-signed value for StickY, so we adjust for that here
				result = controller.GetLeftThumbStickY();
				emulated_key = result < 0
					               ? APP_PAD_EMUL_LEFT_THUMB_UP
					               : APP_PAD_EMUL_LEFT_THUMB_DOWN;
				return g_thisFrameKeys[emulated_key] ? -result : result;
			case AxisCode::RightStickX:
				return controller.GetRightThumbStickX();
			case AxisCode::RightStickY:
				result = controller.GetRightThumbStickY();
				emulated_key = result < 0
					               ? APP_PAD_EMUL_RIGHT_THUMB_UP
					               : APP_PAD_EMUL_RIGHT_THUMB_DOWN;
				return g_thisFrameKeys[emulated_key] ? -result : result;
			case AxisCode::LeftTrigger:
				return controller.GetLeftTrigger();
			case AxisCode::RightTrigger:
				return controller.GetRightTrigger();
			default:
				throw "Invalid Axis";
		}
	}
	
	bool
	GetButton(ButtonCode a_button, uint8_t a_controller)
	{
		button_underlying_t thisButton = g_thisFrameButtons[a_controller];

		button_underlying_t buttonToCheck = g_inputMap[a_button];

		return (thisButton & buttonToCheck);
	}
	
	bool
	GetButtonDown(ButtonCode a_button, uint8_t a_controller)
	{
		button_underlying_t thisButton = g_thisFrameButtons[a_controller];
		button_underlying_t lastButton = g_lastFrameButtons[a_controller];

		button_underlying_t buttonToCheck = g_inputMap[a_button];

		return (thisButton & buttonToCheck) && !(lastButton & buttonToCheck);
	}
	
	bool
	GetButtonUp(ButtonCode a_button, uint8_t a_controller)
	{
		button_underlying_t thisButton = g_thisFrameButtons[a_controller];
		button_underlying_t lastButton = g_lastFrameButtons[a_controller];

		button_underlying_t buttonToCheck = g_inputMap[a_button];

		return !(thisButton & buttonToCheck) && (lastButton & buttonToCheck);
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

		return g_thisFrameKeys[key] && !g_lastFrameButtons[key];
	}
	
	bool
	GetKeyUp(KeyCode a_key)
	{
		key_underlying_t key = g_inputMap[a_key];

		return !g_thisFrameKeys[key] && g_lastFrameButtons[key];
	}
}
