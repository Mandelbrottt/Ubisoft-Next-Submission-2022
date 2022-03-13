#pragma once

#include <MinimalInclude.h>

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

private:
	float m_fuelAmount;

	//ReflectMembers(
	//	ReflectField()
	//)
};
