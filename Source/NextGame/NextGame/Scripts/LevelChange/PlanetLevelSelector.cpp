#include "PlanetLevelSelector.h"

#include "Scripts/Character/Player/PlayerShip.h"
#include "Scripts/Objects/SpinInPlace.h"

ReflectRegister(PlanetLevelSelector);

using namespace Next;

void
PlanetLevelSelector::OnCreate()
{
	auto collider = AddComponent<SphereCollider>();

	AddComponent<SpinInPlace>();
}

void
PlanetLevelSelector::Init(
	std::string_view const& a_planetFile,
	float                   a_radius,
	float                   a_spinSpeed,
	Reflection::Type const& a_sceneType
)
{
	AddComponent<ModelRenderer>()->model = Model::Create(a_planetFile);

	GetComponent<SphereCollider>()->radius = a_radius;
	GetComponent<SpinInPlace>()->spinSpeed = a_spinSpeed;

	Transform()->SetLocalScale(Vector3(a_radius));

	m_sceneTypeId = a_sceneType.GetTypeId();
}

void
PlanetLevelSelector::OnTriggerCollisionStart(Collider* a_other)
{
	if (a_other->GetEntity() != PlayerShip::GetPlayerShipEntity())
	{
		return;
	}

	SceneManager::LoadScene(m_sceneTypeId);
}
