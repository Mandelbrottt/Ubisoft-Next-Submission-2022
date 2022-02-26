#include "RotateOverTime.h"

#include <MinimalInclude.h>

using namespace Next;

void
RotateOverTime::OnUpdate()
{
	m_theta += Time::DeltaTime();

	auto* transform = Transform();
	auto rotation = transform->GetRotation();

	rotation.x = m_theta;
	rotation.z = m_theta * 0.5f;

	transform->SetRotation(rotation);
}
