#include "PlayerShip.h"

#include <MinimalInclude.h>

#include <Components/Colliders/SphereCollider.h>

#include "PlayerShipController.h"
#include "TractorBeamBehaviour.h"

#include "Scenes/MenuScene.h"

#include "Scripts/Character/Common/Health.h"
#include "Scripts/Objects/FuelPickup.h"

ReflectRegister(PlayerShip);

using namespace Next;

Entity PlayerShip::s_thisEntity;
Entity PlayerShip::s_shipControllerEntity;

void
PlayerShip::OnCreate()
{
	s_thisEntity = GetEntity();

	s_shipControllerEntity = Entity::Create("ShipController");
	auto shipController = s_shipControllerEntity.AddComponent<PlayerShipController>();

	// Become a child of the ShipController because we're just along for the ride, it controls everything
	Transform()->SetParent(s_shipControllerEntity.Transform());

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

	m_playerFuel = AddComponent<PlayerFuelController>();
	m_playerFuel->SetFuel(100);

	AddComponent<PlayerUi>();
}

void
PlayerShip::OnDestroy()
{
	s_thisEntity           = Entity();
	s_shipControllerEntity = Entity();
}

void
PlayerShip::OnFirstUpdate()
{
	Transform()->SetLocalPosition({ 0, 2.f - PlayerShipController::min_y, 0 });
}

void
PlayerShip::OnUpdate()
{
	if (Input::GetButtonDown(GamepadButton::Back))
	{
		SceneManager::LoadScene<MenuScene>();
	}
}