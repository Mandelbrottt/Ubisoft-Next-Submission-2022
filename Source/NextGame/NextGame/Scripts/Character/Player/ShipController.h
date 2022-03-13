#pragma once

#include <MinimalInclude.h>

#include "Scripts/Objects/ProjectileSpawner.h"

class ShipController : public Next::Behaviour
{
	ComponentDeclare(ShipController, Behaviour)

public:
	void
	OnCreate() override;

	void
	OnFirstUpdate() override;

	void
	OnUpdate() override;

	Next::Vector3
	GetVelocity() const
	{
		return m_velocity;
	}

private:
	void
	ProcessPlayerMovement();

	void
	ProcessPlayerRotation();

	void
	ProcessPlayerAttack();

public:
	static Next::Vector3 gravity;
	static float         min_y;

private:
	Next::Transform*   m_transform         = nullptr;
	Next::Transform*   m_cameraTransform   = nullptr;
	ProjectileSpawner* m_projectileSpawner = nullptr;

	Next::Vector3 m_velocity = { 0, 0, 0 };

	Next::Vector3 m_topSpeed;
	Next::Vector3 m_accelerationForce;
	float         m_turnSpeed;

	float m_yaw   = 0;
	float m_pitch = 0;
	float m_roll  = 0;

	float m_attackTimer    = 0;
	float m_attackCooldown;

	ReflectMembers(
		ReflectField(m_transform)
		ReflectField(m_cameraTransform)
		ReflectField(m_projectileSpawner)
		ReflectField(m_topSpeed)
		ReflectField(m_accelerationForce)
		ReflectField(m_turnSpeed)
		ReflectField(m_attackCooldown)
	)
};
