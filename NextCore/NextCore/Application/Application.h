#pragma once

#include <string>

#include "NextCoreCommon.h"

namespace Next::Application
{
	/**
	 * \return The path to the resource directory where images and sounds can be loaded
	 */
	NEXT_CORE_EXPORT
	extern
	std::string
	ResourcePath();
}
