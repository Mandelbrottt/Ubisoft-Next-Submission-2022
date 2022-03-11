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
	Next::Transform* m_transform = nullptr;

	Next::Vector3 m_velocity = { 0, 0, 0 };

	Next::Vector3 m_topSpeed          = { 10, 10, 10 };
	Next::Vector3 m_accelerationForce = { 8, 15, 8 };
	float         m_turnSpeed         = 180;

	float m_yaw   = 0;
	float m_pitch = 0;
	
	ReflectMembers(
		ReflectField(m_transform)
	)
};
