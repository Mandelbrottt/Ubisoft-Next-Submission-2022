#include "pch.h"

#include "MinimalInclude.h"

#include "Game.h"

#include "Player.h"

#include <NextAPI/app.h>

// TODO: Decouple Sound, Write wrapper for drawing and printing to screen

using namespace NextCore;

std::vector<Scripting::Entity> g_entities;

void
GameInit()
{
	g_entities.reserve(100);
	
	{
		g_entities.emplace_back();
		Scripting::Entity& player = g_entities.back();
		player.AddComponent<Player>();
	}
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void
GameUpdate()
{
	//------------------------------------------------------------------------
	// Example Sprite Code....
	for (auto& entity : g_entities)
	{
		entity.OnUpdate();
	}
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void
GameRender()
{
	//------------------------------------------------------------------------
	// Example Line Drawing.
	//------------------------------------------------------------------------
	static float a = 0.0f;
	float        r = 1.0f;
	float        g = 1.0f;
	float        b = 1.0f;
	a += 10 * Time::DeltaTime();
	for (int i = 0; i < 20; i++)
	{
		float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
		float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
		float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
		float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
		g        = (float) i / 20.0f;
		b        = (float) i / 20.0f;
		App::DrawLine(sx, sy, ex, ey, r, g, b);
	}

	for (auto& entity : g_entities)
	{
		auto* sprite = entity.GetComponent<Sprite>();

		if (!sprite || !sprite->IsValid()) continue;

		sprite->Render();
	}

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");
}

void
GameShutdown()
{

}