#include "Game.h"

#include <MinimalInclude.h>
#include <algorithm>

#include <Components/LineRenderer.h>

#include <NextAPI/app.h>

#include "CrazyLineThing.h"
#include "Cube.h"
#include "Player.h"

// TODO: Decouple Sound, Write wrapper for drawing and printing to screen

using namespace NextCore;

std::vector<Scripting::Entity> g_entities;

void
GameInit()
{
	g_entities.reserve(100);
	
	{
		//g_entities.emplace_back();
		//Scripting::Entity& player = g_entities.back();
		//player.AddComponent(Player::GetType());
		//player.AddComponent(CrazyLineThing::GetType());
	}
	{
		g_entities.emplace_back();
		Scripting::Entity& cube = g_entities.back();
		cube.AddComponent(Cube::GetType());
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
		int lineRendererCount;
		auto* lineRenderers = entity.GetComponents<Component::LineRenderer>(&lineRendererCount);
		
		if (lineRenderers)
		{
			for (int i = 0; i < lineRendererCount; i++)
			{
				lineRenderers[i]->OnRender();
			}
		}

		int spriteCount;
		auto* sprites = entity.GetComponents<Graphics::Sprite>(&spriteCount);
		
		if (sprites)
		{
			for (int i = 0; i < spriteCount; i++)
			{
				sprites[i]->OnPreRender();
			}
			
			auto predicate = [](Graphics::Sprite* a_lhs, Graphics::Sprite* a_rhs)
			{
				// Depth is front-to-back 1-0
				return a_lhs->GetDepth() > a_rhs->GetDepth();
			};
			
			std::sort(sprites, sprites + spriteCount, predicate);
			
			for (int i = 0; i < spriteCount; i++)
			{
				sprites[i]->OnRender();
			}
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