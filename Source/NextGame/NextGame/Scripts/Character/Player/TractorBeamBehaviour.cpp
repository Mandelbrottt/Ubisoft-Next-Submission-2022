#include "TractorBeamBehaviour.h"

ReflectRegister(TractorBeamBehaviour);

using namespace Next;

void
TractorBeamBehaviour::OnCreate()
{
	m_transform = Transform();
}

void
TractorBeamBehaviour::OnUpdate()
{
	UpdatePickingUpTimer();

	float turnSpeed = Math::Lerp(m_pickingUpTimer, m_normalTurnSpeed, m_pickingUpTurnSpeed);
	
	m_rotation += turnSpeed * Time::DeltaTime();

	m_transform->SetLocalRotation(Vector3(0, 0, m_rotation));
}

void
TractorBeamBehaviour::UpdatePickingUpTimer()
{
	float valueToAdd = Time::DeltaTime() * m_pickingUpTimerMultiplier;
	if (!isPickingUp)
	{
		valueToAdd = -valueToAdd;
	}

	m_pickingUpTimer += valueToAdd;

	m_pickingUpTimer = std::clamp(m_pickingUpTimer, 0.f, 1.f);
}
