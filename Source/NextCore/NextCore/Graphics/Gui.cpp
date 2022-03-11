#include "pch.h"
#include "Gui.h"

#include "Application/Application.h"

namespace Next::Gui
{
	struct TextPrintInfo
	{
		std::string text;
		Vector2     position;
		Color       color;
	};
	
	static std::vector<TextPrintInfo> g_textPrintInfos;

	void
	Flush()
	{
		for (auto const& [text, position, color] : g_textPrintInfos)
		{
			App::Print(
				position.x,
				position.y,
				text.c_str(),
				color.r,
				color.g,
				color.b
			);
		}

		g_textPrintInfos.clear();
	}
	
	void
	Print(std::string a_text, Vector2 a_position, Color a_color)
	{
		g_textPrintInfos.push_back({
			std::move(a_text),
			a_position,
			a_color
		});
	}

	void
	Print(std::string a_text, Vector2 a_position, CoordType a_coordType, Color a_color)
	{
		if (a_coordType == CoordType::Screen)
		{
			a_position.x /= APP_VIRTUAL_WIDTH;
			a_position.y /= APP_VIRTUAL_HEIGHT;

			a_position = a_position * 2.f - Vector2(1);
		}
		
		Print(a_text, a_position, a_color);
	}
}
