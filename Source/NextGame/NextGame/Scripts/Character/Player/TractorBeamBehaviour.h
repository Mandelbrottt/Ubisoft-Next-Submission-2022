#pragma once

#include <MinimalInclude.h>

class TractorBeamBehaviour : public Next::Behaviour
{
	ComponentDeclare(TractorBeamBehaviour, Next::Behaviour)

public:
	void
	OnCreate() override;

	void
	OnUpdate() override;

	void
	UpdatePickingUpTimer();

private:
	Next::Transform* m_transform;

	float m_rotation = 0;

	float m_normalTurnSpeed    = 90;
	float m_pickingUpTurnSpeed = 270;
	float m_pickingUpTimer     = 0;

	ReflectMembers(
		ReflectField(m_transform)
		ReflectField(m_normalTurnSpeed)
		ReflectField(m_pickingUpTurnSpeed)
	)
};
