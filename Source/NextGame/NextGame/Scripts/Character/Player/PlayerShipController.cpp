#include "PlayerShipController.h"

#include "Config/Controls.h"

#include "Scripts/Objects/FuelPickup.h"
#include "Scripts/Objects/GravitySource.h"

ReflectRegister(PlayerShipController);

using namespace Next;

bool  PlayerShipController::flatPlanet = false;
float PlayerShipController::min_y      = 1;

void
PlayerShipController::OnCreate()
{
	m_transform = Transform();

	m_gravityCalculator = AddComponent<GravityCalculator>();
	m_accelMover        = AddComponent<AccelerationBasedMover>();

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
	
	m_gravity = m_gravityCalculator->CalculateGravity(flatPlanet);
	
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

	acceleration += m_gravity;

	m_accelMover->ApplyAcceleration(acceleration);
	
	m_accelMover->SetClamp(m_topSpeed);

	m_accelMover->Move();
	
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

	if (Vector::MagnitudeSquared(m_gravity) > Math::Square(5))
	{
		// Calculate the amount by which to rotate to stabilize the ship
		Vector3 relativeRight = m_transform->Right();
		Vector3 worldUp       = Vector::Normalize(-m_gravity);
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
