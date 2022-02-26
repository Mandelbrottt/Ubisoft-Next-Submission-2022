#include "Game.h"

#include <MinimalInclude.h>
#include <algorithm>
#include <functional>

#include <Components/AudioSource.h>
#include <Components/LineRenderer.h>

#include <Graphics/Model.h>

#include <Math/Transformations.h>

#include <Rendering/Renderer.h>

#include <NextAPI/app.h>

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

class Something : public Behaviour
{
	GenerateConstructors(Something)

public:
	AudioSource* 
	GetAudioSource() const { return m_audioSource; }

	void
	SetAudioSource(AudioSource* a_value) { m_audioSource = a_value; }

public:
	AudioSource* m_audioSource = nullptr;
	
	REFLECT_DECLARE(Something, Behaviour)

	REFLECT_MEMBERS(
		REFLECT_FIELD(m_audioSource)
	)
};

REFLECT_REGISTER(Something);

struct ComponentReference
{
	using StaticTypeId = Reflection::TypeId;
	
	Entity* owningId;
	Entity* referenceId;

	StaticTypeId componentTypeId;

	Reflection::Field* field;

	void const* referenceValue;
};

void Foo()
{	
	Entity entitySomething;
	Something* something = entitySomething.AddComponent<Something>();
	
	Entity entityAudio1;
	auto* source1 = entityAudio1.AddComponent<AudioSource>();
	source1->Load(Application::ResourcePath() + "Test.wav");
	
	Entity entityAudio2;
	auto* source2 = entityAudio2.AddComponent<AudioSource>();
	source2->Load(Application::ResourcePath() + "Test 2.wav");

	something->SetAudioSource(source1);
	
	auto& somethingType = Something::GetType();
	
	std::vector<ComponentReference> compRefs;
	
	for (auto& [name, field] : somethingType.instanceFields)
	{
		auto fieldId = field.fieldTypeId;
		auto fieldType = Reflection::Type::TryGet(fieldId);

		Reflection::TypeId componentTypeId = Reflection::GetTypeId<Component>();

		if (fieldType->IsConvertibleTo(componentTypeId))
		{
			Component* comp = field.GetValue<Component*>(something);

			ComponentReference ref;
			ref.owningId        = something->GetEntity();
			ref.referenceId     = comp->GetEntity();
			ref.componentTypeId = fieldId;
			ref.field           = &field;
			ref.referenceValue  = comp;
			
			compRefs.push_back(std::move(ref));
		}
	}

	entityAudio1.AddComponent<Something>();
	entityAudio1.RemoveComponent<AudioSource>();
	entityAudio1.AddComponent<AudioSource>();
	
	for (auto& [owningId, referenceId, componentTypeId, field, referenceValue] : compRefs)
	{
		Component* comp = owningId->GetComponent(somethingType);

		void const* cachedValue = field->GetValue<Component*, Component*>(comp);

		void const* currentValue = referenceId->GetComponent(componentTypeId);
		
		if (cachedValue != currentValue)
		{
			field->SetValue(comp, currentValue);
		}
	}
}