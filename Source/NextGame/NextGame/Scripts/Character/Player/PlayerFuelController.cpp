#include "PlayerFuelController.h"

#include "Scripts/Objects/FuelPickup.h"

ReflectRegister(PlayerFuelController);

using namespace Next;

bool
PlayerFuelController::HasFuel()
{
	return m_fuel > 0;
}

float
PlayerFuelController::GetFuel() const
{
	return m_fuel;
}

void
PlayerFuelController::SetFuel(float a_fuel)
{
	m_fuel = std::max(a_fuel, 0.0f);
}

void
PlayerFuelController::ConsumeFuelPickup(FuelPickup* a_pickup)
{
	// Add the pickup's fuel to our total and destroy it
	
	if (!a_pickup)
	{
		return;
	}
	
	m_fuel += a_pickup->GetFuelAmount();

	a_pickup->GetEntity().Destroy();
}

void
PlayerFuelController::ProcessFuelUsage(Vector3 a_input)
{
	// Reduce our own fuel based on our inputs
	
	float mag = a_input.Magnitude();

	float fuelConsumption = mag * m_fuelUsageRate;

	m_fuel -= fuelConsumption * Time::DeltaTime();

	m_fuel = std::max(m_fuel, 0.0f);
}
