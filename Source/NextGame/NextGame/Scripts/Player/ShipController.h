#pragma once

#include <MinimalInclude.h>

#include "Scripts/Projectile/ProjectileSpawner.h"

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

	void
	ProcessPlayerAttack();

public:
	static Next::Vector3 gravity;

private:
	Next::Transform* m_transform           = nullptr;
	ProjectileSpawner* m_projectileSpawner = nullptr;
	
	Next::Vector3 m_velocity = { 0, 0, 0 };

	Next::Vector3 m_topSpeed          = { 15, 15, 15 };
	Next::Vector3 m_accelerationForce = { 15, 30, 15 };
	float         m_turnSpeed         = 180;

	float m_yaw   = 0;
	float m_pitch = 0;

	float m_attackTimer = 0;
	float m_attackCooldown = 0.25f;
	
	ReflectMembers(
		ReflectField(m_transform)
		ReflectField(m_projectileSpawner)
	)
};
