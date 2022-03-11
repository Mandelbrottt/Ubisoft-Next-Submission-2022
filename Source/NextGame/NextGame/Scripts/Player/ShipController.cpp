#include "ShipController.h"

ReflectRegister(ShipController);

using namespace Next;

void
ShipController::OnCreate()
{
	m_transform = Transform();

	m_velocity          = Vector3(0);
	m_topSpeed          = 5;
	m_accelerationForce = 5;
	m_turnSpeed         = 180;
	m_yaw               = 0;

	m_tankControls = false;
}

void
ShipController::OnUpdate()
{
	ProcessPlayerMovement();

	ProcessPlayerRotation();

	static bool areBothBumpersPressed = false;

	if (!areBothBumpersPressed && 
		Input::GetButton(GamepadButton::LeftBumper) && 
		Input::GetButton(GamepadButton::RightBumper))
	{
		m_tankControls ^= 1;

		areBothBumpersPressed = true;
	}

	if (Input::GetButtonUp(GamepadButton::LeftBumper) || Input::GetButtonUp(GamepadButton::RightBumper))
	{
		areBothBumpersPressed = false;
	}
}

void
ShipController::ProcessPlayerMovement()
{
	Vector2 input;
	input.x = Input::GetAxis(Axis::Horizontal);
	input.y = Input::GetAxis(Axis::Vertical);

	Vector3 sidewaysAcceleration;
	Vector3 forwardAcceleration;
	
	if (m_tankControls)
	{
		sidewaysAcceleration = m_transform->Right() * input.x;
		forwardAcceleration  = m_transform->Forward() * input.y;
	} else
	{
		sidewaysAcceleration = Vector3::Right() * input.x;
		forwardAcceleration  = Vector3::Forward() * input.y;
	}

	Vector3 acceleration = forwardAcceleration + sidewaysAcceleration;

	if (acceleration.MagnitudeSquared() != 0)
	{
		acceleration.Normalize();
	}
	
	acceleration *= m_accelerationForce;

	float deltaTime = Time::DeltaTime();

	m_velocity += acceleration * deltaTime;

	m_velocity.x = std::clamp(m_velocity.x, -m_topSpeed, m_topSpeed);
	m_velocity.z = std::clamp(m_velocity.z, -m_topSpeed, m_topSpeed);
	
	auto position = m_transform->GetPosition();
	
	position += m_velocity * deltaTime;
	
	m_transform->SetPosition(position);
}

void
ShipController::ProcessPlayerRotation()
{
	Vector2 input;
	input.x = Input::GetAxis(Axis::HorizontalLook);
	input.y = Input::GetAxis(Axis::VerticalLook);

	if (m_tankControls)
	{
		m_yaw += input.x * m_turnSpeed * Time::DeltaTime();

		// Keep rotation in check
		if (m_yaw < -360)
		{
			m_yaw += 360;
		}

		if (m_yaw > 360)
		{
			m_yaw -= 360;
		}
	} else
	{
		if (input.x != 0 && input.y != 0)
		{
			m_yaw = Math::Atan2(input.y, input.x);
		}
	}
	
	m_transform->SetRotation({ 0, m_yaw, 0 });
}
