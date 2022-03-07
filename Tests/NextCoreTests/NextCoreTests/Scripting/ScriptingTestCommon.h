#pragma once

namespace Next::Detail
{
	extern int      g_componentPoolDefaultSize;
	extern Registry g_mainEntityRegistry;
	
	extern void SimulateEntityUpdate();
	extern void ResetRegistryAndEntityProperties();
}

namespace Scripting
{
	static
	void
	ScriptingTestStateInit()
	{
		Next::Detail::ResetRegistryAndEntityProperties();
	}
}
