#include "HeightLevelChangeLogic.h"

ReflectRegister(HeightLevelChangeLogic);

void
HeightLevelChangeLogic::OnCreate()
{
	m_transform = Transform();
}

bool
HeightLevelChangeLogic::ShouldChangeLevel()
{
	auto thisPosition   = m_transform->GetPosition();
	auto targetPosition = m_heightTarget->GetPosition();

	return thisPosition.y > targetPosition.y;
}

void
HeightLevelChangeLogic::SetHeightTarget(Next::Transform* a_heightTarget)
{
	m_heightTarget = a_heightTarget;
}
