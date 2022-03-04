#pragma once

namespace Next
{
	/**
	 * \brief Represents a color using the 32R 32G 32B 32A color format.
	 */
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
		
		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;
		float a = 1.0f;

		// TODO: Add more colors
		static Color const white;
		static Color const black;
		static Color const magenta;
	};
}