#pragma once

#include <MinimalInclude.h>

#include "PlayerShip.h"
#include "TractorBeamBehaviour.h"

#include "Scripts/Objects/ProjectileSpawner.h"

class PlayerShipController : public Next::Behaviour
{
	ComponentDeclare(PlayerShipController, Behaviour)

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
	
	void
	ProcessTractorBeam();

public:
	static Next::Vector3 gravity;
	static float         min_y;

private:
	PlayerShip*        m_playerShip;
	Next::Transform*   m_transform         = nullptr;
	Next::Transform*   m_cameraTransform   = nullptr;

	Health*               m_playerHealth = nullptr;
	PlayerFuelController* m_playerFuelController     = nullptr;
	ProjectileSpawner*    m_projectileSpawner        = nullptr;
	TractorBeamBehaviour* m_tractorBeam              = nullptr;
	Next::ModelRenderer*  m_tractorBeamModelRenderer = nullptr;

	Next::Vector3 m_velocity = { 0, 0, 0 };

	Next::Vector3 m_topSpeed;
	Next::Vector3 m_accelerationForce;
	float         m_turnSpeed;
	
	float m_yaw   = 0;
	float m_pitch = 0;
	float m_roll  = 0;

	float m_attackTimer = 0;
	float m_attackCooldown;
	float m_tractorBeamRange;

	ReflectMembers(
		ReflectField(m_transform)
		ReflectField(m_cameraTransform)
		
		ReflectField(m_projectileSpawner)
		ReflectField(m_tractorBeam)
		ReflectField(m_tractorBeamModelRenderer)

		ReflectField(m_topSpeed)
		ReflectField(m_accelerationForce)
		ReflectField(m_turnSpeed)
		
		ReflectField(m_attackCooldown)
		ReflectField(m_tractorBeamRange)
	)
};
