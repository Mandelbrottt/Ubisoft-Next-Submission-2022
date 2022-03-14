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

	auto turretRenderer   = AddComponent<ModelRenderer>();
	turretRenderer->model = Model::Create("objects/turret.obj");

	auto collider = AddComponent<SphereCollider>();
	collider->radius = 3;

	auto health = AddComponent<Health>();
	health->SetHealth(1);
}

void
TurretFireController::OnFirstUpdate()
{
	m_playerTransform = PlayerShip::GetPlayerShipEntity().Transform();
	m_shipController = PlayerShip::GetPlayerControllerEntity().GetComponent<PlayerShipController>();

	m_startingUp = Transform()->Up();
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
	// Rotate around the local y axis to face the player
	auto predictedPosition = 
		m_playerTransform->GetPosition() + m_shipController->GetVelocity() * CalculatePredictiveFireCoefficient();

	m_aimDirection = predictedPosition - Transform()->GetPosition();
	m_aimDirection.Normalize();

	auto rotation = Matrix3(Matrix::LookAt(Vector3::Zero(), m_aimDirection, m_startingUp));
	Vector3 relativeUp = rotation[1];
	
	auto cross = Vector::Cross(relativeUp, m_startingUp);
	auto angle = Vector::Angle(relativeUp, m_startingUp);
	
	rotation *= Matrix::Rotate(angle, cross);
	
	Transform()->SetLocalRotation(rotation);
}

void
TurretFireController::TryFireTurret()
{
	// Only fire every few seconds
	if (m_fireTimer < m_fireCooldown)
	{
		m_fireTimer += Time::DeltaTime();
		return;
	}

	// Between 1 and 3 seconds
	m_fireCooldown = Random::Value() * 2 + 1;

	auto position = Transform()->GetPosition() + Transform()->Up();

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
