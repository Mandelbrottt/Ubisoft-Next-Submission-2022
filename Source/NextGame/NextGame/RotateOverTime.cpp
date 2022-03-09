#include "RotateOverTime.h"

#include <MinimalInclude.h>

using namespace Next;

ReflectRegister(RotateOverTime);

void
RotateOverTime::OnCreate()
{
	m_turnSpeed = 30 + (rand() % 500) / 10.f;
	m_turnSpeed = 100;
}

void
RotateOverTime::OnUpdate()
{
	m_theta += m_turnSpeed * Time::DeltaTime();

	auto* transform = Transform();
	auto rotation = transform->GetRotation();

	rotation.x = m_theta;
	rotation.y = m_theta * 0.25f;
	rotation.z = m_theta * 0.5f;

	transform->SetRotation(rotation);
}
