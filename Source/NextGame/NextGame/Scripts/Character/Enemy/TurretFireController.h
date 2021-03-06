#pragma once

#include <MinimalInclude.h>

#include "Scripts/Character/Player/PlayerShipController.h"
#include "Scripts/Objects/ProjectileSpawner.h"

class TurretFireController : public Next::Behaviour
{
	ComponentDeclare(TurretFireController, Next::Behaviour)

public:
	void
	OnCreate() override;

	void
	OnFirstUpdate() override;

	void
	OnUpdate() override;

private:
	void
	AimTurret();

	void
	TryFireTurret();

	float
	CalculatePredictiveFireCoefficient();

private:
	Next::Vector3 m_startingUp;

	Next::Transform* m_playerTransform;
	ProjectileSpawner* m_projectileSpawner;

	PlayerShipController const* m_shipController;
	
	Next::Vector3 m_aimDirection;

	float m_fireTimer    = 0;
	float m_fireCooldown = 5;

	ReflectMembers(
		ReflectField(m_playerTransform)
		ReflectField(m_projectileSpawner)
		ReflectField(m_shipController)
	)
};
