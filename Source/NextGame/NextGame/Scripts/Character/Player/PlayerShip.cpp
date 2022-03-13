#include "PlayerShip.h"

#include <MinimalInclude.h>

#include <Components/Colliders/SphereCollider.h>

#include "ShipController.h"
#include "TractorBeamBehaviour.h"

#include "Scenes/MenuScene.h"

#include "Scripts/Character/Common/Health.h"
#include "Scripts/Objects/FuelPickup.h"

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

	// Create camera-relative objects
	{
		Entity crosshair = Entity::Create("Crosshair");
	
		auto crosshairModel = crosshair.AddComponent<ModelRenderer>();
		crosshairModel->model = Model::Create("objects/crosshair.obj");

		crosshair.Transform()->SetLocalScale({ 2, 2, 2 });
		crosshair.Transform()->SetLocalRotation({ 0, 180, 0 });
		crosshair.Transform()->SetParent(camera->Transform());

		Entity tractorBeam = Entity::Create("TractorBeam");

		tractorBeam.AddComponent<TractorBeamBehaviour>();
		
		auto tractorBeamModel = tractorBeam.AddComponent<ModelRenderer>();
		tractorBeamModel->model = Model::Create("objects/tractorbeam.obj");

		tractorBeam.Transform()->SetLocalPosition({ 0, 0, 0.5f });
		tractorBeam.Transform()->SetLocalScale({ 2, 2, 2 });
		//tractorBeam.Transform()->SetLocalRotation({ 0, 180, 0 });
		tractorBeam.Transform()->SetParent(camera->Transform());
	}
	
	auto collider = AddComponent<SphereCollider>();
	collider->radius = 2;

	m_health = AddComponent<Health>();
	m_health->SetHealth(5);
	m_health->DestroyOnKill(false);

	m_fuel = 100;
}

void
PlayerShip::OnFirstUpdate()
{
	Transform()->SetLocalPosition({ 0, 2.f - ShipController::min_y, 0 });
}

void
PlayerShip::OnUpdate()
{
	if (Input::GetButtonDown(GamepadButton::Back))
	{
		SceneManager::LoadScene<MenuScene>();
	}
}

void
PlayerShip::ConsumeFuel(FuelPickup* a_pickup)
{
	if (!a_pickup)
	{
		return;
	}
	
	m_fuel += a_pickup->GetFuelAmount();

	a_pickup->GetEntity().Destroy();
}
