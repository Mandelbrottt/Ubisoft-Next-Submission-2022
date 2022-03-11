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

public:
	static Next::Vector3 gravity;

private:
	Next::Transform* m_transform;

	Next::Vector3 m_velocity;

	Next::Vector3 m_topSpeed;
	Next::Vector3 m_accelerationForce;
	float         m_turnSpeed;

	float m_yaw;
	float m_pitch;

	ReflectMembers(
		ReflectField(m_transform)
	)
};
