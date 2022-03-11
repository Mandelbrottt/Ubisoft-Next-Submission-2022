#include "PlayerShip.h"

#include <MinimalInclude.h>

#include "ShipController.h"

#include "Scenes/MenuScene.h"

ReflectRegister(PlayerShip);

using namespace Next;

void
PlayerShip::OnCreate()
{
	m_shipController = AddComponent<ShipController>();
	
	auto camera = AddComponent<Camera>();
	camera->SetFov(100, FovType::Horizontal);

}

void
PlayerShip::OnUpdate()
{
	if (Input::GetButtonDown(GamepadButton::Back))
	{
		SceneManager::LoadScene<MenuScene>();
	}
}
