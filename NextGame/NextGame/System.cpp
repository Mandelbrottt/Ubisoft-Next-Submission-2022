#include "pch.h"

#include "Game.h"
#include "NextCoreCommon.h"

void
Init()
{
	GameInit();
}

// Declare Time::Update manually so as to not expose it unnecessarily
namespace NextCore::Time
{
	NEXT_CORE_EXPORT
	extern
	void
	Update(float a_deltaTime);
}

void
Update(float a_deltaTime)
{
	// Convert deltaTime into seconds because NextAPI uses milliseconds
	float timeInSeconds = a_deltaTime / 1000.f;
	NextCore::Time::Update(timeInSeconds);

	GameUpdate();
}

void
Render()
{
	GameRender();
}

void
Shutdown()
{
	GameShutdown();
}
