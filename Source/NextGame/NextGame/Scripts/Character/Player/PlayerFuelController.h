#pragma once

#include <MinimalInclude.h>

class FuelPickup;

class PlayerFuelController : public Next::Behaviour
{
	ComponentDeclare(PlayerFuelController, Next::Behaviour)

public:
	bool
	HasFuel();

	float
	GetFuel() const;

	void
	SetFuel(float a_fuel);

	void
	ConsumeFuelPickup(FuelPickup* a_pickup);

	void
	ProcessFuelUsage(Next::Vector3 a_input);

private:
	float m_fuel          = 100;
	float m_fuelUsageRate = 1;
	
	ReflectMembers(
		ReflectField(m_fuel)
		ReflectField(m_fuelUsageRate)
	)
};
