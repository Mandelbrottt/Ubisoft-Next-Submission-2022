#include "pch.h"

#include "Init.h"

#include "Scripting/Entity.h"

void
Init()
{
	Application_Init();
}

// Declare System Updates manually so as to not expose it unnecessarily
namespace Next
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
	Next::Time::Update(timeInSeconds);

	Next::Input::Update();

	Next::Entity::Update();
}

void
Render()
{
	
}

void
Shutdown()
{
	
}
