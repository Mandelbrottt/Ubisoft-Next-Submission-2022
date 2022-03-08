#include "pch.h"

#include "Application.h"

// declarations for NextAPI window width and height.
// DO NOT MODIFY THESE VALUES, these should be read only.
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

namespace Next::Application
{
	std::string ResourcePath()
	{
		return NEXT_RESOURCE_DIR;
	}

	Vector2
	ScreenSize()
	{
		return { ScreenWidth(), ScreenHeight() };
	}

	float
	ScreenWidth()
	{
		return static_cast<float>(WINDOW_WIDTH);
	}

	float
	ScreenHeight()
	{
		return static_cast<float>(WINDOW_HEIGHT);
	}
}
