#include "PlayerShip.h"

#include <MinimalInclude.h>

#include <Components/Colliders/SphereCollider.h>

#include "ShipController.h"

#include "Scenes/MenuScene.h"

#include "Scripts/Character/Common/Health.h"

ReflectRegister(PlayerShip);

using namespace Next;

void
PlayerShip::OnCreate()
{
	m_shipControllerEntity = Entity::Create("ShipController");
	m_shipController = m_shipControllerEntity.AddComponent<ShipController>();

	// Become a child of the ShipController because we're just along for the ride, it controls everything
	Transform()->SetParent(m_shipControllerEntity.Transform());

	Entity cameraEntity = Entity::Create("Camera");
	cameraEntity.Transform()->SetParent(Transform());
	
	auto camera = cameraEntity.AddComponent<Camera>();
	camera->SetFov(100, FovType::Horizontal);

	auto collider = AddComponent<SphereCollider>();
	collider->radius = 2;

	m_health = AddComponent<Health>();
	m_health->SetHealth(5);
	m_health->DestroyOnKill(false);
}

void
PlayerShip::OnFirstUpdate()
{
	Transform()->SetLocalPosition({ 0, 2.f, 0 });
}

void
PlayerShip::OnUpdate()
{
	if (Input::GetButtonDown(GamepadButton::Back))
	{
		SceneManager::LoadScene<MenuScene>();
	}
}
