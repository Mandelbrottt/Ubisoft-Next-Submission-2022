#pragma once

#include <MinimalInclude.h>

class ShipController : public Next::Behaviour
{
	ComponentDeclare(ShipController, Behaviour)

public:
	void
	OnCreate() override;

	void
	OnUpdate() override;

private:
	void
	ProcessPlayerMovement();

	void
	ProcessPlayerRotation();

private:
	Next::Transform* m_transform;

	Next::Vector3 m_velocity;

	float m_topSpeed;
	float m_accelerationForce;
	float m_turnSpeed;

	float m_yaw;

	bool m_tankControls;

	ReflectMembers(
		ReflectField(m_transform)
	)
};
