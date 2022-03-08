#include "pch.h"

#include "Application.h"

#include <filesystem>

// declarations for NextAPI window width and height.
// DO NOT MODIFY THESE VALUES, these should be read only.
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

namespace Next::Application
{
	static std::filesystem::path g_exeDirectoryOnStartup = std::filesystem::current_path();
	
	std::string const& ResourcePath()
	{
		static std::string result = []()->std::string
		{
			auto resourcePath = g_exeDirectoryOnStartup / NEXT_RESOURCE_DIR;
			return absolute(resourcePath).string();
		}();

		return result;
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
