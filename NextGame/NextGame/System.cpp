#include "Game.h"

void
Init()
{
	GameInit();
}

// Declare System Updates manually so as to not expose it unnecessarily
namespace NextCore
{
	namespace Time
	{
		extern
		void
		Update(float a_deltaTime);
	}

	namespace Input
	{
		extern
		void
		Update();
	}
}

void
Update(float a_deltaTime)
{
	// Convert deltaTime into seconds because NextAPI uses milliseconds
	float timeInSeconds = a_deltaTime / 1000.f;
	NextCore::Time::Update(timeInSeconds);

	NextCore::Input::Update();

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