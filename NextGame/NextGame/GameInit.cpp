#include <MinimalInclude.h>

#include <Application/Init.h>

#include <Components/AudioSource.h>

#include <Graphics/Model.h>

#include <Rendering/Renderer.h>

#include "Cube.h"
#include "RotateOverTime.h"

// TODO: Write wrapper printing to screen

using namespace Next;

void
Application_Init()
{
	Model cube;
	cube.LoadFromFile(Application::ResourcePath() + "cube/plane.obj");

	Model suzanne;
	suzanne.LoadFromFile(Application::ResourcePath() + "complex/suzanne.obj");

	Entity cubeEntity = Entity::Create();
	cubeEntity.AddComponent<Cube>();
	cubeEntity.Transform()->SetPosition({ 0, 0, 0 });

	for (int i = 0; i <= 10; i++)
	{
		float angle = 360.f * i / 10.f;

		float x = 4 * std::cos(angle);
		float y = 4 * std::sin(angle);

		Entity entity = Entity::Create();
		//entity.AddComponent<RotateOverTime>();
		auto* innerModelRenderer = entity.AddComponent<ModelRenderer>();

		innerModelRenderer->model = suzanne;

		entity.Transform()->SetPosition({ x, y, 10 });
	}
}

//class Something : public Behaviour
//{
//	GenerateConstructors(Something)
//
//public:
//	AudioSource* 
//	GetAudioSource() const { return m_audioSource; }
//
//	void
//	SetAudioSource(AudioSource* a_value) { m_audioSource = a_value; }
//
//public:
//	AudioSource* m_audioSource = nullptr;
//	
//	REFLECT_DECLARE(Something, Behaviour)
//
//	REFLECT_MEMBERS(
//		REFLECT_FIELD(m_audioSource)
//	)
//};
//
//REFLECT_REGISTER(Something);
//
//struct ComponentReference
//{
//	using StaticTypeId = Reflection::TypeId;
//	
//	Entity* owningId;
//	Entity* referenceId;
//
//	StaticTypeId componentTypeId;
//
//	Reflection::Field* field;
//
//	void const* referenceValue;
//};
//
//void ReferenceTracking()
//{	
//	Entity entitySomething;
//	Something* something = entitySomething.AddComponent<Something>();
//	
//	Entity entityAudio1;
//	auto* source1 = entityAudio1.AddComponent<AudioSource>();
//	source1->Load(Application::ResourcePath() + "Test.wav");
//	
//	Entity entityAudio2;
//	auto* source2 = entityAudio2.AddComponent<AudioSource>();
//	source2->Load(Application::ResourcePath() + "Test 2.wav");
//
//	something->SetAudioSource(source1);
//	
//	auto& somethingType = Something::GetType();
//	
//	std::vector<ComponentReference> compRefs;
//	
//	for (auto& [name, field] : somethingType.instanceFields)
//	{
//		auto fieldId = field.fieldTypeId;
//		auto fieldType = Reflection::Type::TryGet(fieldId);
//
//		Reflection::TypeId componentTypeId = Reflection::GetTypeId<Component>();
//
//		if (fieldType->IsConvertibleTo(componentTypeId))
//		{
//			Component* comp = field.GetValue<Component*>(something);
//
//			ComponentReference ref;
//			ref.owningId        = something->GetEntity();
//			ref.referenceId     = comp->GetEntity();
//			ref.componentTypeId = fieldId;
//			ref.field           = &field;
//			ref.referenceValue  = comp;
//			
//			compRefs.push_back(std::move(ref));
//		}
//	}
//
//	entityAudio1.AddComponent<Something>();
//	entityAudio1.RemoveComponent<AudioSource>();
//	entityAudio1.AddComponent<AudioSource>();
//	
//	for (auto& [owningId, referenceId, componentTypeId, field, referenceValue] : compRefs)
//	{
//		Component* comp = owningId->GetComponent(somethingType);
//
//		void const* cachedValue = field->GetValue<Component*, Component*>(comp);
//
//		void const* currentValue = referenceId->GetComponent(componentTypeId);
//		
//		if (cachedValue != currentValue)
//		{
//			field->SetValue(comp, currentValue);
//		}
//	}
//}