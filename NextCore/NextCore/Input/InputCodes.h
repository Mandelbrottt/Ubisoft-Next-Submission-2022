#pragma once

#include <cstdint>

#ifdef _WIN32
	#include "Windows.h"
	#include "Xinput.h"
#endif

namespace NextCore::Input
{
	enum class Axis : uint8_t
	{
		LeftStickX,
		Horizontal     = LeftStickX,
		LeftStickY,
		Vertical       = LeftStickY,
		RightStickX,
		HorizontalLook = RightStickX,
		RightStickY,
		VerticalLook   = RightStickY,
		LeftTrigger,
		L2             = LeftTrigger,
		RightTrigger,
		R2             = RightTrigger,
	};

	enum class Button : uint16_t
	{
		South         = XINPUT_GAMEPAD_A,
		Cross         = South,
		A             = South,
		West          = XINPUT_GAMEPAD_B,
		Circle        = West,
		B             = West,
		North         = XINPUT_GAMEPAD_Y,
		Triangle      = North,
		Y             = North,
		East          = XINPUT_GAMEPAD_X,
		X             = East,
		Square        = East,
		DPadUp        = XINPUT_GAMEPAD_DPAD_UP,
		DPadDown      = XINPUT_GAMEPAD_DPAD_DOWN,
		DPadLeft      = XINPUT_GAMEPAD_DPAD_LEFT,
		DPadRight     = XINPUT_GAMEPAD_DPAD_RIGHT,
		LeftShoulder  = XINPUT_GAMEPAD_LEFT_SHOULDER,
		LeftBumper    = LeftShoulder,
		L1            = LeftShoulder,
		RightShoulder = XINPUT_GAMEPAD_RIGHT_SHOULDER,
		RightBumper   = RightShoulder,
		R1            = RightShoulder,
		LeftStick     = XINPUT_GAMEPAD_LEFT_THUMB,
		RightStick    = XINPUT_GAMEPAD_RIGHT_THUMB,
		Start         = XINPUT_GAMEPAD_START,
		Menu          = Start,
		Select        = XINPUT_GAMEPAD_BACK,
		Back          = Select,
	};

	// TODO: make more portable
	enum class Key : uint8_t
	{
		A            = 'A',
		B            = 'B',
		C            = 'C',
		D            = 'D',
		E            = 'E',
		F            = 'F',
		G            = 'G',
		H            = 'H',
		I            = 'I',
		J            = 'J',
		K            = 'K',
		L            = 'L',
		M            = 'M',
		N            = 'N',
		O            = 'O',
		P            = 'P',
		Q            = 'Q',
		R            = 'R',
		S            = 'S',
		T            = 'T',
		U            = 'U',
		V            = 'V',
		W            = 'W',
		X            = 'X',
		Y            = 'Y',
		Z            = 'Z',
		Shift        = VK_SHIFT,
		LeftShift    = Shift,
		RightShift   = VK_SHIFT,
		Control      = VK_CONTROL,
		LeftControl  = Control,
		RightControl = VK_CONTROL,
		CapsLock     = VK_CAPITAL,
		Tab          = VK_TAB,
		Escape       = VK_ESCAPE,
		Grave        = VK_OEM_3,      // `~
		Tilde        = Grave,		  
		Up           = VK_UP,		  
		UpArrow      = Up,			  
		Down         = VK_DOWN,		  
		DownArrow    = Down,		  
		Left         = VK_LEFT,		  
		LeftArrow    = Left,		  
		Right        = VK_RIGHT,	  
		RightArrow   = Right,		  
		Alpha1       = '1',			  
		Alpha2       = '2',			  
		Alpha3       = '3',			  
		Alpha4       = '4',			  
		Alpha5       = '5',			  
		Alpha6       = '6',			  
		Alpha7       = '7',			  
		Alpha8       = '8',			  
		Alpha9       = '9',			  
		Alpha0       = '0',			  
		Return       = VK_RETURN,	  
		Enter        = Return,		  
		Comma        = VK_OEM_COMMA,  // '<
		Period       = VK_OEM_PERIOD, // .>
		Slash        = VK_OEM_2,      // /?
		QuestionMark = Slash,
		Backslash    = VK_OEM_5,      // \|
		Pipe         = Backslash,
		Semicolon    = VK_OEM_1,      // ;:
		Colon        = Semicolon,
		Apostrophe   = VK_OEM_7,      // '"
		Quote        = Apostrophe,
		Minus        = VK_OEM_MINUS,  // -_
		Underscore   = Minus,
		Equals       = VK_OEM_PLUS,   // =+
		Plus         = Equals,
		Numpad1      = VK_NUMPAD0,
		Numpad2      = VK_NUMPAD1,
		Numpad3      = VK_NUMPAD2,
		Numpad4      = VK_NUMPAD3,
		Numpad5      = VK_NUMPAD4,
		Numpad6      = VK_NUMPAD5,
		Numpad7      = VK_NUMPAD6,
		Numpad8      = VK_NUMPAD7,
		Numpad9      = VK_NUMPAD8,
		Numpad0      = VK_NUMPAD9,
	};
}
