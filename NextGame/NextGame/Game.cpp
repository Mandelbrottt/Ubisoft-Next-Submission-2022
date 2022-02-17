#include "MinimalInclude.h"

#include "Game.h"

#include <Components/LineRenderer.h>

#include "Player.h"

#include <NextAPI/app.h>

#include "CrazyLineThing.h"

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
		player.AddComponent(Player::GetType());
		player.AddComponent(CrazyLineThing::GetType());
	}
}

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

void
GameRender()
{	
	for (auto& entity : g_entities)
	{
		if (auto* sprite = entity.GetComponent<Graphics::Sprite>(); sprite && sprite->IsValid())
		{
			sprite->OnRender();
		}

		if (auto* lineRenderer = entity.GetComponent<Component::LineRenderer>(); lineRenderer)
		{
			lineRenderer->OnRender();
		}
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