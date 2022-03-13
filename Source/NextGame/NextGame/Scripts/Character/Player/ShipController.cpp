#include "ShipController.h"

#include "Config/Controls.h"

ReflectRegister(ShipController);

using namespace Next;

Vector3 ShipController::gravity = Vector3::Down() * 9.81f;
float   ShipController::min_y   = 1;

void
ShipController::OnCreate()
{
	m_transform = Transform();

	m_projectileSpawner = AddComponent<ProjectileSpawner>();
	m_projectileSpawner->isEnemyOwned = false;
	m_projectileSpawner->projectileSpeed = 60;
	
	m_topSpeed          = { 15, 15, 15 };
	m_accelerationForce = { 15, 30, 15 };
	m_turnSpeed         = 180;
	
	m_attackCooldown = 0.25f;

}

void
ShipController::OnFirstUpdate()
{
	Entity camera = Entity::FindByName("Camera");
	m_cameraTransform = camera.Transform();
}

void
ShipController::OnUpdate()
{
	ProcessPlayerMovement();

	ProcessPlayerRotation();

	ProcessPlayerAttack();
}

void
ShipController::ProcessPlayerMovement()
{
	Vector3 input;
	input.x = Input::GetAxis(HORIZONTAL_MOVE);
	input.z = Input::GetAxis(FORWARD_MOVE);

	// Subtract left trigger because that's control to go down
	input.y = Input::GetAxis(UPWARDS_MOVE) - Input::GetAxis(DOWNWARDS_MOVE);

	Vector3 sidewaysAcceleration = m_transform->Right()   * input.x;
	Vector3 upwardsAcceleration  = m_transform->Up()      * input.y;
	Vector3 forwardAcceleration  = m_transform->Forward() * input.z;

	Vector3 acceleration = sidewaysAcceleration + upwardsAcceleration + forwardAcceleration;

	if (acceleration.MagnitudeSquared() != 0)
	{
		acceleration.Normalize();
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

	if (gravity.x == 0 && gravity.z == 0)
	{
		if (position.y < 1)
		{
			position.y = 1;
			m_velocity.y = -m_velocity.y * 0.2f;
		}
	}
	
	m_transform->SetPosition(position);
}

void
ShipController::ProcessPlayerRotation()
{
	Vector2 input;
	input.x = Input::GetAxis(HORIZONTAL_LOOK);
	input.y = Input::GetAxis(VERTICAL_LOOK);
	
	m_pitch += input.y * m_turnSpeed * Time::DeltaTime();

	if (!Input::GetButton(ROLL_MODIFIER))
	{
		m_yaw += input.x * m_turnSpeed * Time::DeltaTime();
	} else
	{
		m_roll += input.x * m_turnSpeed * Time::DeltaTime();
	}

	// Keep rotation in check
	if (m_yaw < -360)
	{
		m_yaw += 360;
	}

	if (m_yaw > 360)
	{
		m_yaw -= 360;
	}
	
	//m_pitch = std::clamp(m_pitch, -89.f, 89.f);

	//m_transform->SetLocalRotation({ 0, m_yaw, 0 });
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

	Vector3 relativeUp = m_transform->Up();
	Vector3 worldUp = Vector::Normalize(-gravity);

	float angle = Vector::Angle(relativeUp, worldUp);

	// Don't need to normalize result because Matrix::Rotate normalizes for us
	Vector3 rotationAxis = Vector::Cross(relativeUp, worldUp);
	
	float factor = 1.f;

	if (false && rotationAxis != Vector3::Zero())
	{
		auto rotation = m_transform->GetLocalRotation();
		rotation *= Matrix::Rotate(angle * factor * Time::DeltaTime(), rotationAxis);
		m_transform->SetLocalRotation(rotation);
	}
	
	//m_transform->SetLocalRotation(Matrix::Rotate(m_yaw, m_cameraTransform->Up()));
	//m_cameraTransform->SetLocalRotation(Matrix::Rotate(m_pitch, m_cameraTransform->Right()));
	//m_cameraTransform->SetLocalRotation({ m_pitch, 0, 0 });

	m_yaw = m_pitch = m_roll = 0;
}

void
ShipController::ProcessPlayerAttack()
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
		auto position = m_cameraTransform->GetPosition();
		
		m_projectileSpawner->SpawnProjectile(position + direction, direction);
		m_attackTimer = 0;
	}
}
