#pragma once

#include <MinimalInclude.h>

class AccelerationBasedMover : public Next::Behaviour
{
	ComponentDeclare(AccelerationBasedMover, Next::Behaviour)

public:
	Next::Vector3
	GetVelocity() const
	{
		return m_velocity;
	}

	void
	SetClamp(Next::Vector3 a_clamp);
	
	void
	ApplyAcceleration(Next::Vector3 a_acceleration);

	// Apply the velocity to this component's position
	void
	Move();

	void
	OnTriggerCollisionStart(Next::Collider* a_other) override;
	
	void
	OnTriggerCollision(Next::Collider* a_other) override;

private:
	Next::Vector3 m_velocity = Next::Vector3(0);
	
	Next::Vector3 m_clamp   = Next::Vector3::Zero();

	//ReflectMembers(
	//	ReflectField()
	//)
};
