#pragma once

#include <MinimalInclude.h>

#include "PlayerFuelController.h"

#include "Scripts/Character/Common/Health.h"

class PlayerUi : public Next::Behaviour
{
	ComponentDeclare(PlayerUi, Next::Behaviour)

public:
	void
	OnFirstUpdate() override;

	void
	OnUpdate() override;

private:
	Health*               m_playerHealth   = nullptr;
	PlayerFuelController* m_fuelController = nullptr;

	//ReflectMembers(
	//	ReflectField()
	//)
};
