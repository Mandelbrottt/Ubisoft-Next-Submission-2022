#pragma once

#include <HeaderInclude.h>

#include "ShipController.h"

class PlayerShip : public Next::Behaviour
{
	ComponentDeclare(PlayerShip, Behaviour)

public:
	void
	OnCreate() override;

	void
	OnUpdate() override;

private:
	ShipController* m_shipController;

	ReflectMembers(
		ReflectField(m_shipController)
	)
};
