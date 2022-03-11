#include "ShipController.h"

ReflectRegister(ShipController);

using namespace Next;

Vector3 ShipController::gravity = Vector3::Down() * 2.f;

void
ShipController::OnCreate()
{
	m_transform = Transform();
}

void
ShipController::OnUpdate()
{
	ProcessPlayerMovement();

	ProcessPlayerRotation();
}

void
ShipController::ProcessPlayerMovement()
{
	Vector3 input;
	input.x = Input::GetAxis(Axis::Horizontal);
	input.z = Input::GetAxis(Axis::Vertical);

	// Subtract left trigger because that's control to go down
	input.y = Input::GetAxis(Axis::RightTrigger) - 0.5f * Input::GetAxis(Axis::LeftTrigger);

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
	
	m_transform->SetPosition(position);
}

void
ShipController::ProcessPlayerRotation()
{
	Vector2 input;
	input.x = Input::GetAxis(Axis::HorizontalLook);
<<<<<<< HEAD
	input.y = Input::GetAxis(Axis::VerticalLook);
	
	m_yaw += input.x * m_turnSpeed * Time::DeltaTime();
=======
	input.y = Input::GetAxis(Axis::VerticalLook) * 0.8f;

	m_pitch += input.y * m_turnSpeed * Time::DeltaTime();
	m_yaw   += input.x * m_turnSpeed * Time::DeltaTime();
>>>>>>> d1565ba119d8c0ceeef11cf68d3f11d37d1360bc

	// Keep rotation in check
	if (m_yaw < -360)
	{
		m_yaw += 360;
	}

	if (m_yaw > 360)
	{
		m_yaw -= 360;
	}

<<<<<<< HEAD
	m_pitch += input.y * m_turnSpeed * Time::DeltaTime();
	
=======
>>>>>>> d1565ba119d8c0ceeef11cf68d3f11d37d1360bc
	m_pitch = std::clamp(m_pitch, -89.f, 89.f);

	m_transform->SetRotation({ m_pitch, m_yaw, 0 });
}