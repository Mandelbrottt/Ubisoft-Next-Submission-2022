#pragma once

#include <HeaderInclude.h>

#include "PlayerFuelController.h"
#include "PlayerUI.h"

#include "Scripts/Character/Common/Health.h"

class FuelPickup;

class PlayerShip : public Next::Behaviour
{
	ComponentDeclare(PlayerShip, Behaviour)

public:
	void
	OnCreate() override;

	void
	OnDestroy() override;

	void
	OnFirstUpdate() override;

	void
	OnUpdate() override;

	static
	Next::Entity
	GetPlayerShipEntity()
	{
		return s_thisEntity;
	}

	static
	Next::Entity
	GetPlayerControllerEntity()
	{
		return s_shipControllerEntity;
	}

private:
	static Next::Entity s_thisEntity;
	static Next::Entity s_shipControllerEntity;

	//Next::Entity m_shipControllerEntity;
	//ShipController* m_shipController = nullptr;

	Health*               m_health     = nullptr;
	PlayerFuelController* m_playerFuel = nullptr;

	ReflectMembers(
		//ReflectField(m_shipController)
		ReflectField(m_health)
		ReflectField(m_playerFuel)
	)
};
