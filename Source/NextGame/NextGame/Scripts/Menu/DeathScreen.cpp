#include "DeathScreen.h"

#include "Scenes/MenuScene.h"

#include "Scripts/Character/Player/PlayerShip.h"

ReflectRegister(DeathScreen);

using namespace Next;

void
DeathScreen::OnFirstUpdate()
{
	auto player = PlayerShip::GetPlayerShipEntity();
	if (!(player.IsValid() && player.IsActive()))
	{
		return;
	}

	m_playerHealth = player.GetComponent<Health>();
}

void
DeathScreen::OnUpdate()
{
	if (!m_playerHealth)
	{
		return;
	}

	if (m_playerHealth->GetHealth() <= 0)
	{
		Gui::Print("You Died!", { -0.02f, 0.6 }, Color(1.0f, 0.3f, 0.3f));
		Gui::Print("Press Start to play again!", { -0.1f, 0.4 }, Color(1.0f, 0.3f, 0.3f));

		if (Input::GetButtonDown(GamepadButton::Start))
		{
			SceneManager::LoadScene<MenuScene>();
		}
	}
}
