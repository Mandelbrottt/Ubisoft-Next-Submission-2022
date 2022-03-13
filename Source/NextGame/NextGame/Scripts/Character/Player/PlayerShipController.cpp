#include "PlayerShipController.h"

#include "Config/Controls.h"

#include "Scripts/Objects/FuelPickup.h"

ReflectRegister(PlayerShipController);

using namespace Next;

Vector3 PlayerShipController::gravity = Vector3::Zero();
float   PlayerShipController::min_y   = 1;

void
PlayerShipController::OnCreate()
{
	m_transform = Transform();

	m_projectileSpawner                  = AddComponent<ProjectileSpawner>();
	m_projectileSpawner->isEnemyOwned    = false;
	m_projectileSpawner->projectileSpeed = 60;

	m_topSpeed          = { 30, 30, 30 };
	m_accelerationForce = { 15, 30, 15 };
	m_turnSpeed         = 180;

	m_attackCooldown   = 0.25f;
	m_tractorBeamRange = 25;
}

void
PlayerShipController::OnFirstUpdate()
{
	Entity camera     = Entity::FindByName("Camera");
	m_cameraTransform = camera.Transform();

	Entity tractorBeam = Entity::FindByName("TractorBeam");
	m_tractorBeam      = tractorBeam.GetComponent<TractorBeamBehaviour>();

	m_tractorBeamModelRenderer = m_tractorBeam->GetComponent<ModelRenderer>();
	m_tractorBeamModelRenderer->SetActive(false);

	m_playerFuelController = PlayerShip::GetPlayerShipEntity().GetComponent<PlayerFuelController>();

	m_playerHealth = PlayerShip::GetPlayerShipEntity().GetComponent<Health>();
}

void
PlayerShipController::OnUpdate()
{
	auto cachedPosition = m_transform->GetPosition();
	auto cachedRotation = m_transform->GetLocalRotation();

	// We handle non-input movement in here so dont if-guard. TODO: Move non-input movement out of this function
	ProcessPlayerMovement();

	if (m_playerHealth->GetHealth() > 0)
	{
		ProcessPlayerRotation();

		ProcessPlayerAttack();

		ProcessTractorBeam();
	}

	// Sometimes the values are nan and i don't know why, just pretend it didnt happen
	auto right = m_transform->Right();
	if (isnan(right.x) || isnan(right.y) || isnan(right.z))
	{
		m_transform->SetPosition(cachedPosition);
		m_transform->SetLocalRotation(cachedRotation);
	}
}

void
PlayerShipController::ProcessPlayerMovement()
{
	Vector3 input;
	input.x = Input::GetAxis(HORIZONTAL_MOVE);
	input.z = Input::GetAxis(FORWARD_MOVE);

	// combine upwards and downwards move into one command
	input.y = Input::GetAxis(UPWARDS_MOVE) - Input::GetAxis(DOWNWARDS_MOVE);

	Vector3 acceleration = Vector3(0);
	
	if (m_playerFuelController->HasFuel() && m_playerHealth->GetHealth() > 0)
	{
		Vector3 sidewaysAcceleration = m_transform->Right() * input.x;
		Vector3 upwardsAcceleration  = m_transform->Up() * input.y;
		Vector3 forwardAcceleration  = m_transform->Forward() * input.z;

		acceleration = sidewaysAcceleration + upwardsAcceleration + forwardAcceleration;

		if (acceleration.MagnitudeSquared() != 0)
		{
			acceleration.Normalize();
		}
	}

	acceleration *= m_accelerationForce;

	acceleration += gravity;

	float deltaTime = Time::DeltaTime();

	m_velocity += acceleration * deltaTime;

	m_velocity.x = std::clamp(m_velocity.x, -m_topSpeed.x, m_topSpeed.x);
	m_velocity.y = std::clamp(m_velocity.y, -m_topSpeed.y, m_topSpeed.y);
	m_velocity.z = std::clamp(m_velocity.z, -m_topSpeed.z, m_topSpeed.z);

	auto position = m_transform->GetPosition();

	position += m_velocity * deltaTime;

	if (gravity != Vector3::Zero())
	{
		Vector3 planetToThis = position - Vector3::Zero();
		
		float dist = Vector::MagnitudeSquared(planetToThis);
		
		if (dist < min_y)
		{
			planetToThis.Normalize();
			
			position = planetToThis * min_y;
			if (m_velocity.Dot(-planetToThis) > 10)
			{
				m_playerHealth->SubtractHealth();
			}

			m_velocity = -m_velocity * 0.2f;
		}
	}

	m_transform->SetPosition(position);

	m_playerFuelController->ProcessFuelUsage(input);
}

void
PlayerShipController::ProcessPlayerRotation()
{
	Vector2 input;
	input.x = Input::GetAxis(HORIZONTAL_LOOK);
	input.y = Input::GetAxis(VERTICAL_LOOK);

	m_pitch = input.y * m_turnSpeed * Time::DeltaTime();

	if (!Input::GetButton(ROLL_MODIFIER))
	{
		m_yaw = input.x * m_turnSpeed * Time::DeltaTime();
	} else
	{
		m_roll = input.x * m_turnSpeed * Time::DeltaTime();
	}

	if (m_roll != 0)
	{
		auto rotation = m_transform->GetLocalRotation();
		rotation *= Matrix::Rotate(-m_roll, m_transform->Forward());
		m_transform->SetRotation(rotation);
	}

	if (m_yaw != 0)
	{
		auto rotation = m_transform->GetLocalRotation();
		rotation *= Matrix::Rotate(m_yaw, m_transform->Up());
		m_transform->SetRotation(rotation);
	}

	if (m_pitch != 0)
	{
		auto rotation = m_transform->GetLocalRotation();
		rotation *= Matrix::Rotate(-m_pitch, m_transform->Right());
		m_transform->SetRotation(rotation);
	}

	// Calculate the amount by which to rotate to stabilize the ship
	Vector3 relativeRight = m_transform->Right();
	Vector3 worldUp       = Vector::Normalize(-gravity);
	Vector3 worldRight    = Vector::Cross(worldUp, m_transform->Forward());

	float angle = Vector::Angle(relativeRight, worldRight);

	// Don't need to normalize result because Matrix::Rotate normalizes for us
	Vector3 rotationAxis = Vector::Cross(relativeRight, worldRight);

	float factor = 2.f - (m_roll != 0 ? 1.5f : 0);

	angle = std::min(angle, 30.f);

	if (rotationAxis != Vector3::Zero())
	{
		auto rotation = m_transform->GetLocalRotation();
		angle         = std::min(angle, angle * factor * Time::DeltaTime());
		rotation *= Matrix::Rotate(angle, rotationAxis);
		m_transform->SetLocalRotation(rotation);
	}

	m_yaw = m_pitch = m_roll = 0;
}

void
PlayerShipController::ProcessPlayerAttack()
{
	if (m_attackTimer < m_attackCooldown)
	{
		m_attackTimer += Time::DeltaTime();
		return;
	}

	bool input = Input::GetButton(GamepadButton::RightBumper);

	if (input)
	{
		auto direction = m_cameraTransform->Forward();
		auto position  = m_cameraTransform->GetPosition();

		m_projectileSpawner->SpawnProjectile(position + direction, direction);
		m_attackTimer = 0;
	}
}

void
PlayerShipController::ProcessTractorBeam()
{
	static std::vector<FuelPickup*> fuelPickups;

	if (Input::GetButtonDown(TRACTOR_BEAM))
	{
		m_tractorBeamModelRenderer->SetActive(true);
	}

	if (Input::GetButton(TRACTOR_BEAM))
	{
		m_tractorBeam->isPickingUp = false;

		Entity::GetAllComponents(fuelPickups);

		auto position = m_transform->GetPosition();

		for (auto pickup : fuelPickups)
		{
			auto pickupPos        = pickup->Transform()->GetPosition();
			auto toPickupFromThis = pickupPos - position;

			auto dot = Vector::Dot(Vector::Normalize(toPickupFromThis), m_transform->Forward());

			if (toPickupFromThis.MagnitudeSquared() < Math::Square(m_tractorBeamRange) &&
			    dot > std::cos(35 * Math::DEG_TO_RAD))
			{
				m_tractorBeam->isPickingUp = true;
				pickup->SetAccelerationTarget(m_transform);
			} else
			{
				pickup->SetAccelerationTarget(nullptr);
			}
		}
	}

	if (Input::GetButtonUp(TRACTOR_BEAM))
	{
		m_tractorBeamModelRenderer->SetActive(false);
		m_tractorBeam->isPickingUp = false;

		Entity::GetAllComponents(fuelPickups);
		for (auto pickup : fuelPickups)
		{
			pickup->SetAccelerationTarget(nullptr);
		}
	}
}
