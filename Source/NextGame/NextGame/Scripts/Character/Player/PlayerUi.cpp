#include "PlayerUI.h"

ReflectRegister(PlayerUi);

using namespace Next;

void
PlayerUi::OnFirstUpdate()
{
	m_playerHealth = GetComponent<Health>();
	m_fuelController = GetComponent<PlayerFuelController>();
}

void
PlayerUi::OnUpdate()
{
	char healthText[128];
	sprintf_s(healthText, "Health: %.1f", m_playerHealth->GetHealth());

	char fuelText[128];
	sprintf_s(fuelText, "Fuel: %.2f", m_fuelController->GetFuel());

	Gui::Print(healthText, { -0.97, 0.90 });
	Gui::Print(fuelText,   { -0.97, 0.80 });
}
