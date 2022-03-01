#pragma once

namespace Next
{
	struct Color
	{
		constexpr
		Color() = default;

		constexpr
		Color(float a_r, float a_g, float a_b, float a_a = 1.0f)
			: r(a_r),
			  g(a_g),
			  b(a_b),
			  a(a_a) { }
		
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		float a = 1.0f;

		// TODO: Add more colors
		static Color const white;
		static Color const black;
		static Color const magenta;
	};
}