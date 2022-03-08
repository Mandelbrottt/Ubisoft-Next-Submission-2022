#pragma once

#include <string>

#include "NextCoreCommon.h"

#include "Math/Vector2.h"

namespace Next::Application
{
	/**
	 * \return The path to the resource directory where images and sounds can be loaded
	 */
	NEXT_CORE_EXPORT
	extern
	std::string const&
	ResourcePath();

	/**
	 * \return The screen size in pixels 
	 */
	NEXT_CORE_EXPORT
	extern
	Vector2
	ScreenSize();
	
	/**
	 * \return The screen width in pixels 
	 */
	NEXT_CORE_EXPORT
	extern
	float
	ScreenWidth();
	
	/**
	 * \return The screen height in pixels 
	 */
	NEXT_CORE_EXPORT
	extern
	float
	ScreenHeight();
}
