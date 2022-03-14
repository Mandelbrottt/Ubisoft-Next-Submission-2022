#pragma once

#include <MinimalInclude.h>

#include <Components/AudioSource.h>
#include <Components/AudioSource.h>

class GravityCalculator : public Next::Behaviour
{
	ComponentDeclare(GravityCalculator, Next::Behaviour)

public:
	void
	OnCreate() override;

	Next::Vector3
	CalculateGravity(bool a_flatPlanet = false) const;

private:
	Next::Transform* m_transform = nullptr;

	ReflectMembers(
		ReflectField(m_transform)
	)
};
