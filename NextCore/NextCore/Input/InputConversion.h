#pragma once

#include "InputCodes.h"

#include <type_traits>
#include <array>

#ifdef _WIN32
	#include <Windows.h>
	#include <Xinput.h>
#endif

namespace Next::Input::Detail
{
	template<typename TEnum, typename TElement, int Size>
	struct EnumIndexedArray
	{
		using underlying_t = std::underlying_type_t<TEnum>;
		
		std::array<TElement, Size> array;

		constexpr
		EnumIndexedArray()
			: array() {}

		constexpr
		TElement&
		operator [](TEnum a_enum)
		{
			underlying_t index = enum_cast(a_enum);
			return array[index];
		}
		
		constexpr
		TElement
		operator [](TEnum a_enum) const
		{
			underlying_t index = enum_cast(a_enum);
			return array.at(index);
		}

	private:
		constexpr
		static
		underlying_t
		enum_cast(TEnum a_enum)
		{
			return static_cast<underlying_t>(a_enum);
		}
	};

	struct InputMap
	{
		using key_underlying_t          = std::underlying_type_t<KeyCode>;
		using mouse_button_underlying_t = std::underlying_type_t<MouseButton>;

	#ifdef _WIN32
		// Windows uses bit-masks for XINPUT button codes, so use a different underlying type than ButtonCode 
		using gamepad_button_underlying_t = uint16_t;
	#endif

		constexpr static int key_size          = 256;
		constexpr static int button_size       = 16;
		constexpr static int mouse_button_size = 5;
		
		constexpr
		InputMap();

		constexpr
		key_underlying_t
		operator [](KeyCode a_key) const
		{
			return m_keys[a_key];
		}

		constexpr
		gamepad_button_underlying_t
		operator [](GamepadButton a_gamepadButton) const
		{
			return m_buttons[a_gamepadButton];
		}
		
		constexpr
		mouse_button_underlying_t
		operator [](MouseButton a_mouseButton) const
		{
			return m_mouseButtons[a_mouseButton];
		}

	private:
		EnumIndexedArray<KeyCode, key_underlying_t, key_size> m_keys;

		EnumIndexedArray<GamepadButton, gamepad_button_underlying_t, button_size> m_buttons;

		EnumIndexedArray<MouseButton, mouse_button_underlying_t, mouse_button_size> m_mouseButtons;
	};
	
	constexpr
	InputMap::InputMap()
	{
		// TODO: Make keycodes portable
	#ifdef _WIN32
		// Gamepad buttons
		m_buttons[GamepadButton::DPadUp]        = XINPUT_GAMEPAD_DPAD_UP;
		m_buttons[GamepadButton::DPadDown]      = XINPUT_GAMEPAD_DPAD_DOWN;
		m_buttons[GamepadButton::DPadLeft]      = XINPUT_GAMEPAD_DPAD_LEFT;
		m_buttons[GamepadButton::DPadRight]     = XINPUT_GAMEPAD_DPAD_RIGHT;
		m_buttons[GamepadButton::South]         = XINPUT_GAMEPAD_A;
		m_buttons[GamepadButton::West]          = XINPUT_GAMEPAD_B;
		m_buttons[GamepadButton::East]          = XINPUT_GAMEPAD_X;
		m_buttons[GamepadButton::North]         = XINPUT_GAMEPAD_Y;
		m_buttons[GamepadButton::LeftShoulder]  = XINPUT_GAMEPAD_LEFT_SHOULDER;
		m_buttons[GamepadButton::RightShoulder] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		m_buttons[GamepadButton::LeftStick]     = XINPUT_GAMEPAD_LEFT_THUMB;
		m_buttons[GamepadButton::RightStick]    = XINPUT_GAMEPAD_RIGHT_THUMB;
		m_buttons[GamepadButton::Start]         = XINPUT_GAMEPAD_START;
		m_buttons[GamepadButton::Select]        = XINPUT_GAMEPAD_BACK;

		m_mouseButtons[MouseButton::Left]   = VK_LBUTTON;
		m_mouseButtons[MouseButton::Right]  = VK_RBUTTON;
		m_mouseButtons[MouseButton::Middle] = VK_MBUTTON;
		m_mouseButtons[MouseButton::Mouse4] = VK_XBUTTON1;
		m_mouseButtons[MouseButton::Mouse5] = VK_XBUTTON2;
		
		for (int i = 0; i < 26; i++)
		{
			const key_underlying_t firstLetterCode  = static_cast<key_underlying_t>(KeyCode::A);
			const char             firstLetterValue = 'A';
			
			KeyCode              convertedKeyCode   = static_cast<KeyCode>(firstLetterCode + i);
			key_underlying_t convertedKeyValue  = firstLetterValue + i;

			m_keys[convertedKeyCode] = convertedKeyValue;
		}

		// Alphanumeric Keys
		for (int i = 0; i < 10; i++)
		{
			const key_underlying_t firstNumberCode  = static_cast<key_underlying_t>(KeyCode::A);
			const char             firstNumberValue = '0';
			
			KeyCode              convertedKeyCode   = static_cast<KeyCode>(firstNumberCode + i);
			key_underlying_t convertedKeyValue  = firstNumberValue + i;

			m_keys[convertedKeyCode] = convertedKeyValue;
		}
		
		// Numpad Keys
		for (int i = 0; i < 10; i++)
		{
			const key_underlying_t firstNumberCode  = static_cast<key_underlying_t>(KeyCode::Numpad0);
			const char             firstNumberValue = VK_NUMPAD0;
			
			KeyCode              convertedKeyCode   = static_cast<KeyCode>(firstNumberCode + i);
			key_underlying_t convertedKeyValue  = firstNumberValue + i;

			m_keys[convertedKeyCode] = convertedKeyValue;
		}

		// Special Keys
		m_keys[KeyCode::Space]        = VK_SPACE;
		m_keys[KeyCode::Shift]        = VK_SHIFT;
		m_keys[KeyCode::LeftShift]    = VK_LSHIFT;
		m_keys[KeyCode::RightShift]   = VK_RSHIFT;
		m_keys[KeyCode::Control]      = VK_CONTROL;
		m_keys[KeyCode::LeftControl]  = VK_LCONTROL;
		m_keys[KeyCode::RightControl] = VK_RCONTROL;
		m_keys[KeyCode::CapsLock]     = VK_CAPITAL;
		m_keys[KeyCode::Tab]          = VK_TAB;
		m_keys[KeyCode::Escape]       = VK_ESCAPE;
		m_keys[KeyCode::Up]           = VK_UP;
		m_keys[KeyCode::Down]         = VK_DOWN;
		m_keys[KeyCode::Left]         = VK_LEFT;
		m_keys[KeyCode::Right]        = VK_RIGHT;
		m_keys[KeyCode::Return]       = VK_RETURN;
		m_keys[KeyCode::Semicolon]    = VK_OEM_1;
		m_keys[KeyCode::Plus]         = VK_OEM_PLUS;
		m_keys[KeyCode::Comma]        = VK_OEM_COMMA;
		m_keys[KeyCode::Minus]        = VK_OEM_MINUS;
		m_keys[KeyCode::Period]       = VK_OEM_PERIOD;
		m_keys[KeyCode::Slash]        = VK_OEM_2;
		m_keys[KeyCode::Grave]        = VK_OEM_3;
		m_keys[KeyCode::LeftBracket]  = VK_OEM_4;
		m_keys[KeyCode::Backslash]    = VK_OEM_5;
		m_keys[KeyCode::RightBracket] = VK_OEM_6;
		m_keys[KeyCode::Apostrophe]   = VK_OEM_7;
	#endif
	}
}
