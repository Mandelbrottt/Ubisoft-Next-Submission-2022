#pragma once

#include <cstdint>

namespace Next
{
	enum class AxisCode : uint8_t
	{
		// [-1, 1]
		LeftStickX,
		Horizontal     = LeftStickX,

		// [-1, 1]
		LeftStickY,
		Vertical       = LeftStickY,

		// [-1, 1]
		RightStickX,
		HorizontalLook = RightStickX,

		// [-1, 1]
		RightStickY,
		VerticalLook   = RightStickY,

		// [0, 1]
		LeftTrigger,
		L2             = LeftTrigger,

		// [0, 1]
		RightTrigger,
		R2             = RightTrigger,
	};

	enum class ButtonCode : uint16_t
	{
		South,
		Cross         = South,
		A             = South,
		West,		  
		Circle        = West,
		B             = West,
		North,		  
		Triangle      = North,
		Y             = North,
		East,		  
		X             = East,
		Square        = East,
		DPadUp,
		DPadDown,
		DPadLeft,
		DPadRight,
		LeftShoulder,
		LeftBumper    = LeftShoulder,
		L1            = LeftShoulder,
		RightShoulder,
		RightBumper   = RightShoulder,
		R1            = RightShoulder,
		LeftStick,	  
		RightStick,	  
		Start,		  
		Menu          = Start,
		Select,		  
		Back          = Select,
	};
	
	enum class KeyCode : uint8_t
	{
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Alpha1,
		Alpha2,
		Alpha3,
		Alpha4,
		Alpha5,
		Alpha6,
		Alpha7,
		Alpha8,
		Alpha9,
		Alpha0,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		Space,
		Shift,
		LeftShift,
		RightShift,
		Control,
		LeftControl,
		RightControl,
		CapsLock,
		Tab,
		Escape,
		Grave,						// `~
		Tilde        = Grave,
		Up,
		UpArrow      = Up,
		Down,
		DownArrow    = Down,
		Left,
		LeftArrow    = Left,
		Right,
		RightArrow   = Right,
		Return,
		Enter        = Return,
		Comma,						// '<
		Period,						// .>
		Slash,						// /?
		QuestionMark = Slash,
		Backslash,					// \|
		Pipe         = Backslash,
		LeftBracket,
		RightBracket,
		Semicolon,					// ;:
		Colon        = Semicolon,
		Apostrophe,					// '"
		Quote        = Apostrophe,
		Minus,						// -_
		Underscore   = Minus,
		Equals,						// =+
		Plus         = Equals,
	};
}
