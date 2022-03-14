#pragma once

#include <MinimalInclude.h>

#include "Scripts/Character/Player/PlayerShipController.h"

class FuelPickup : public Next::Behaviour
{
	ComponentDeclare(FuelPickup, Next::Behaviour)

public:
	void
	OnCreate() override;

	void
	OnUpdate() override;

	float
	GetFuelAmount() const;

	void
	SetFuelAmount(float a_fuelAmount);

	void
	SetAccelerationTarget(Next::Transform* a_target);

	void
	OnTriggerCollisionStart(Next::Collider* a_other) override;

private:
	Next::Transform*   m_transform          = nullptr;
	Next::Transform*   m_accelerationTarget = nullptr;
	GravityCalculator* m_gravityCalculator  = nullptr;
	
	Next::Vector3 m_velocity = Next::Vector3(0);
	Next::Vector3 m_localGravity;
	
	float m_fuelAmount = 0;
	
	ReflectMembers(
		ReflectField(m_transform)
		ReflectField(m_accelerationTarget)
		ReflectField(m_gravityCalculator)
	)
};
