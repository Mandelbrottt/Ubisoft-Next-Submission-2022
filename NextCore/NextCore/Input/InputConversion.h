#pragma once

#include "InputCodes.h"

#include <type_traits>
#include <array>

#ifdef _WIN32
	#include <Windows.h>
	#include <Xinput.h>
#endif

namespace NextCore::Input::Detail
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
		using key_underlying_t = std::underlying_type_t<Key>;
		using button_underlying_t = std::underlying_type_t<Button>;

		constexpr static int key_size    = 256;
		constexpr static int button_size = 16;
		
		constexpr
		InputMap();

		constexpr
		key_underlying_t
		operator [](Key a_key) const
		{
			return m_keys[a_key];
		}

		constexpr
		button_underlying_t
		operator [](Button a_button) const
		{
			return m_buttons[a_button];
		}

	private:
		EnumIndexedArray<Key, key_underlying_t, key_size> m_keys;

		EnumIndexedArray<Button, button_underlying_t, button_size> m_buttons;
	};
	
	constexpr
	InputMap::InputMap()
	{
		// TODO: Make keycodes portable
	#ifdef _WIN32
		// Gamepad buttons
		m_buttons[Button::DPadUp]        = XINPUT_GAMEPAD_DPAD_UP;
		m_buttons[Button::DPadDown]      = XINPUT_GAMEPAD_DPAD_DOWN;
		m_buttons[Button::DPadLeft]      = XINPUT_GAMEPAD_DPAD_LEFT;
		m_buttons[Button::DPadRight]     = XINPUT_GAMEPAD_DPAD_RIGHT;
		m_buttons[Button::South]         = XINPUT_GAMEPAD_A;
		m_buttons[Button::West]          = XINPUT_GAMEPAD_B;
		m_buttons[Button::East]          = XINPUT_GAMEPAD_X;
		m_buttons[Button::North]         = XINPUT_GAMEPAD_Y;
		m_buttons[Button::LeftShoulder]  = XINPUT_GAMEPAD_LEFT_SHOULDER;
		m_buttons[Button::RightShoulder] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		m_buttons[Button::LeftStick]     = XINPUT_GAMEPAD_LEFT_THUMB;
		m_buttons[Button::RightStick]    = XINPUT_GAMEPAD_RIGHT_THUMB;
		m_buttons[Button::Start]         = XINPUT_GAMEPAD_START;
		m_buttons[Button::Select]        = XINPUT_GAMEPAD_BACK;

		for (int i = 0; i < 26; i++)
		{
			const key_underlying_t firstLetterCode  = static_cast<key_underlying_t>(Key::A);
			const char             firstLetterValue = 'A';
			
			Key              convertedKeyCode   = static_cast<Key>(firstLetterCode + i);
			key_underlying_t convertedKeyValue  = firstLetterValue + i;

			m_keys[convertedKeyCode] = convertedKeyValue;
		}

		// Alphanumeric Keys
		for (int i = 0; i < 10; i++)
		{
			const key_underlying_t firstNumberCode  = static_cast<key_underlying_t>(Key::A);
			const char             firstNumberValue = '0';
			
			Key              convertedKeyCode   = static_cast<Key>(firstNumberCode + i);
			key_underlying_t convertedKeyValue  = firstNumberValue + i;

			m_keys[convertedKeyCode] = convertedKeyValue;
		}
		
		// Numpad Keys
		for (int i = 0; i < 10; i++)
		{
			const key_underlying_t firstNumberCode  = static_cast<key_underlying_t>(Key::Numpad0);
			const char             firstNumberValue = VK_NUMPAD0;
			
			Key              convertedKeyCode   = static_cast<Key>(firstNumberCode + i);
			key_underlying_t convertedKeyValue  = firstNumberValue + i;

			m_keys[convertedKeyCode] = convertedKeyValue;
		}

		// Special Keys
		m_keys[Key::Space]        = VK_SPACE;
		m_keys[Key::Shift]        = VK_SHIFT;
		m_keys[Key::LeftShift]    = VK_LSHIFT;
		m_keys[Key::RightShift]   = VK_RSHIFT;
		m_keys[Key::Control]      = VK_CONTROL;
		m_keys[Key::LeftControl]  = VK_LCONTROL;
		m_keys[Key::RightControl] = VK_RCONTROL;
		m_keys[Key::CapsLock]     = VK_CAPITAL;
		m_keys[Key::Tab]          = VK_TAB;
		m_keys[Key::Escape]       = VK_ESCAPE;
		m_keys[Key::Up]           = VK_UP;
		m_keys[Key::Down]         = VK_DOWN;
		m_keys[Key::Left]         = VK_LEFT;
		m_keys[Key::Right]        = VK_RIGHT;
		m_keys[Key::Return]       = VK_RETURN;
		m_keys[Key::Semicolon]    = VK_OEM_1;
		m_keys[Key::Plus]         = VK_OEM_PLUS;
		m_keys[Key::Comma]        = VK_OEM_COMMA;
		m_keys[Key::Minus]        = VK_OEM_MINUS;
		m_keys[Key::Period]       = VK_OEM_PERIOD;
		m_keys[Key::Slash]        = VK_OEM_2;
		m_keys[Key::Grave]        = VK_OEM_3;
		m_keys[Key::LeftBracket]  = VK_OEM_4;
		m_keys[Key::Backslash]    = VK_OEM_5;
		m_keys[Key::RightBracket] = VK_OEM_6;
		m_keys[Key::Apostrophe]   = VK_OEM_7;
	#endif
	}
}
