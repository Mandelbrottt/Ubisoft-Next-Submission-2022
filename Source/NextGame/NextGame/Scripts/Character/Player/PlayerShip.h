#pragma once

#include <HeaderInclude.h>

#include "ShipController.h"

#include "Scripts/Character/Common/Health.h"

class PlayerShip : public Next::Behaviour
{
	ComponentDeclare(PlayerShip, Behaviour)

public:
	void
	OnCreate() override;

	void
	OnFirstUpdate() override;

	void
	OnUpdate() override;

private:
	Next::Entity m_shipControllerEntity;
	ShipController* m_shipController = nullptr;

	Health* m_health = nullptr;

	ReflectMembers(
		ReflectField(m_shipController)
	)
};
