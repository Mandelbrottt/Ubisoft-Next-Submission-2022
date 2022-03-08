#include "RotateOverTime.h"

#include <MinimalInclude.h>

using namespace Next;

ReflectRegister(RotateOverTime);

void
RotateOverTime::OnCreate()
{
	m_turnSpeed = 30 + (rand() % 500) / 10.f;
}

void
RotateOverTime::OnUpdate()
{
	m_theta += Time::DeltaTime();

	auto* transform = Transform();
	auto rotation = transform->GetRotation();

	rotation.x = m_turnSpeed * m_theta;
	rotation.z = m_turnSpeed * m_theta * 0.5f;

	transform->SetRotation(rotation);
}
