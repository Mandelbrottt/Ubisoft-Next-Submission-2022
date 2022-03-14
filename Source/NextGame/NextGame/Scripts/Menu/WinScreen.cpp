#include "WinScreen.h"

#include "Scripts/Character/Enemy/TurretFireController.h"

ReflectRegister(WinScreen);

using namespace Next;

void
WinScreen::OnFirstUpdate()
{
	if (!Entity::GetAllComponents<TurretFireController>().empty())
	{
		m_enemiesExist = true;
	}
}

void
WinScreen::OnUpdate()
{
	if (!m_enemiesExist)
	{
		return;
	}

	static std::vector<TurretFireController*> turrets;
	Entity::GetAllComponents(turrets);

	if (!turrets.empty())
	{
		char enemiesRemainingText[128];
		sprintf_s(enemiesRemainingText, "Enemies Remaining: %llu", turrets.size());

		Gui::Print(enemiesRemainingText, { -0.97, 0.70 });
		return;
	}
	
	Gui::Print("Area Cleared!", { -0.05f, 0.6 }, Color(0.3f, 1.0f, 0.3f));
	Gui::Print("Fly away from the planet and go find some more baddies to kill!", { -0.4f, 0.4 }, Color(0.3f, 1.0f, 0.3f));
}
