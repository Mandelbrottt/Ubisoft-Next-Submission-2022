#include "TurretFireController.h"

#include "Scripts/Character/Common/Health.h"

ReflectRegister(TurretFireController);

using namespace Next;

void
TurretFireController::OnCreate()
{
	m_projectileSpawner = AddComponent<ProjectileSpawner>();
	m_projectileSpawner->isEnemyOwned    = true;
	m_projectileSpawner->projectileSpeed = 40;

	auto collider = AddComponent<SphereCollider>();
	collider->radius = 2;

	auto health = AddComponent<Health>();
	health->SetHealth(1);
}

void
TurretFireController::OnFirstUpdate()
{
	m_playerTransform = Entity::FindByName("Player").Transform();
	m_shipController = Entity::FindByName("ShipController").GetComponent<ShipController>();
}

void
TurretFireController::OnUpdate()
{
	AimTurret();
	TryFireTurret();
}

void
TurretFireController::AimTurret()
{
	auto predictedPosition = 
		m_playerTransform->GetPosition() + m_shipController->GetVelocity() * CalculatePredictiveFireCoefficient();

	m_aimDirection = predictedPosition - Transform()->GetPosition();
	m_aimDirection.Normalize();

	float angle = Math::Atan2(m_aimDirection.z, m_aimDirection.x);

	Transform()->SetLocalRotation({ 0, angle + 180, 0 });
}

void
TurretFireController::TryFireTurret()
{
	if (m_fireTimer < m_fireCooldown)
	{
		m_fireTimer += Time::DeltaTime();
		return;
	}

	// Between 1 and 3 seconds
	m_fireCooldown = Random::Value() * 2 + 1;

	auto position = Transform()->GetPosition() + Vector3::Up();

	m_projectileSpawner->SpawnProjectile(position, m_aimDirection);

	m_fireTimer = 0;
}

float
TurretFireController::CalculatePredictiveFireCoefficient()
{
	Vector3 literalAimVector = m_playerTransform->GetPosition() - Transform()->GetPosition();
	float timeToHit = literalAimVector.Magnitude() / m_projectileSpawner->projectileSpeed;
	return timeToHit;
}
