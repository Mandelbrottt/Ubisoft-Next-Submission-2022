#pragma once
#include "Color.h"

#include "Math/Vector2.h"

namespace Next::Gui
{
	enum class CoordType
	{
		Normalized,
		Screen,
	};

	extern
	void
	Print(std::string a_text, Vector2 a_position, Color a_color = Color::white);

	extern
	void
	Print(std::string a_text, Vector2 a_position, CoordType a_coordType, Color a_color = Color::white);
}
