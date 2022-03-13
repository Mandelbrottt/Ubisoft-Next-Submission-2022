#pragma once

#include <HeaderInclude.h>

#include "ShipController.h"

#include "Scripts/Character/Common/Health.h"

class FuelPickup;

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

	void
	ConsumeFuel(FuelPickup* a_pickup);

private:
	Next::Entity m_shipControllerEntity;
	ShipController* m_shipController = nullptr;

	Health* m_health = nullptr;

	float m_fuel;

	ReflectMembers(
		ReflectField(m_shipController)
		ReflectField(m_health)
		ReflectField(m_fuel)
	)
};
