#include "GravityCalculator.h"

#include "Scripts/Objects/GravitySource.h"

ReflectRegister(GravityCalculator);

using namespace Next;

void
GravityCalculator::OnCreate()
{
	m_transform = Transform();
}

Vector3
GravityCalculator::CalculateGravity(bool a_flatPlanet) const
{
	static std::vector<GravitySource*> gravitySources;

	Vector3 gravity = Vector3::Zero();
	
	if (a_flatPlanet)
	{
		gravity = Vector3::Down() * 9.81f;
		return gravity;
	}

	// If there's no planets, no gravity
	Entity::GetAllComponents(gravitySources);
	if (gravitySources.empty())
	{
		return gravity;
	}

	auto position = m_transform->GetPosition();

	// Apply gravity from all of the planets
	for (auto source : gravitySources)
	{
		Vector3 fromThisToSource = source->Transform()->GetPosition() - position;

		float dist = fromThisToSource.Magnitude();

		float strength = source->gravityStrength / dist;

		if (strength > 0.1f)
		{
			gravity += Vector::Normalize(fromThisToSource) * strength;
		}
	}

	return gravity;
}
