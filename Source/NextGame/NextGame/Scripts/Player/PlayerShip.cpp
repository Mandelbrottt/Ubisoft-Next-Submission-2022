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

	auto modelRenderer = AddComponent<ModelRenderer>();
	modelRenderer->model = Model::Create("complex/suzanne.obj");
}

void
PlayerShip::OnUpdate()
{
	if (Input::GetButtonDown(GamepadButton::Back))
	{
		SceneManager::LoadScene<MenuScene>();
	}
}
