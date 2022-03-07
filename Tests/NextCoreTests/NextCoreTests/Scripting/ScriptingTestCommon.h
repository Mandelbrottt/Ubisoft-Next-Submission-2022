#pragma once

namespace Next::Detail
{
	extern int      g_componentPoolDefaultSize;
	extern Registry g_mainEntityRegistry;
}

namespace Scripting
{
	static
	void
	ScriptingTestStateInit()
	{
		// Reset the main registry to ensure consistent operation
		Next::Detail::g_mainEntityRegistry.Reset();
	}
}
