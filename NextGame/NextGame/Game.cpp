#include "Game.h"

#include <MinimalInclude.h>
#include <algorithm>

#include <Components/LineRenderer.h>

#include <Graphics/Model.h>

#include <Math/Transformations.h>

#include <NextAPI/app.h>

#include <Rendering/Renderer.h>

#include "CrazyLineThing.h"
#include "Cube.h"

// TODO: Decouple Sound, Write wrapper for drawing and printing to screen

using namespace Next;

std::vector<Entity> g_entities;

Model g_model;

void Foo();

void
GameInit()
{
	Foo();
	
	g_entities.reserve(100);

	g_model.LoadFromFile(Application::ResourcePath() + "cube/cube.obj");
	//g_model.LoadFromFile(Application::ResourcePath() + "complex/deer.obj");

	{
		//g_entities.emplace_back();
		//Scripting::Entity& player = g_entities.back();
		//player.AddComponent(Player::GetType());
		//player.AddComponent(CrazyLineThing::GetType());
	}
	{
		g_entities.emplace_back();
		Entity& cube = g_entities.back();
		cube.AddComponent(Cube::GetType());
	}
	for (int i = 0; i <= 10; i++)
	{
		float angle = 360.f * i / 10.f;

		float x = 8 * std::cos(angle);
		float y = 8 * std::sin(angle);

		g_entities.emplace_back();
		Entity& entity = g_entities.back();

		entity.Transform()->SetPosition({ x, y, 10 });
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
		int   lineRendererCount;
		auto* lineRenderers = entity.GetComponents<LineRenderer>(&lineRendererCount);

		if (lineRenderers)
		{
			for (int i = 0; i < lineRendererCount; i++)
			{
				lineRenderers[i]->OnRender();
			}
		}
	}

	// Perspective Matrix
	float fov    = 90;
	float aspect = 16.f / 9.f;

	auto perspective = Matrix::Perspective(fov, aspect, 0.1f, 1000.f);

	Renderer::PrepareScene({}, perspective);

	static float theta = 0;
	theta += Time::DeltaTime();

	for (int i = 0; i < g_entities.size(); i++)
	{
		auto& entity = g_entities[i];

		if (entity.GetComponent<Cube>() == nullptr)
		{
			auto* transform = entity.Transform();
			transform->SetRotation({ theta * 0.5f, 0, theta });
		}

		Renderer::Submit(g_model, g_entities[i]);
	}

	Renderer::Flush();

	//------------------------------------------------------------------------
	// Example Text.
	//------------------------------------------------------------------------
	App::Print(100, 100, "Sample Text");
}

void
GameShutdown() {}

#define PROPERTY(t,n)  __declspec( property ( put = property__set_##n, get = property__get_##n ) ) t n;\
	typedef t property__tmp_type_##n
#define READONLY_PROPERTY(t,n) __declspec( property (get = property__get_##n) ) t n;\
	typedef t property__tmp_type_##n
#define WRITEONLY_PROPERTY(t,n) __declspec( property (put = property__set_##n) ) t n;\
	typedef t property__tmp_type_##n
#define GET(n) property__tmp_type_##n property__get_##n() 
#define SET(n) void property__set_##n(const property__tmp_type_##n& value)

struct Something
{
	PROPERTY(int, x);

	GET(x)
	{
		return x;
	}
	
	SET(x)
	{
		m_x = value;
	}

private:
	int m_x = 0;
};

void Foo()
{
	Something s;
	s.x = 5;
}