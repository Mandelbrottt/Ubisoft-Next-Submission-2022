#pragma once

#include <MinimalInclude.h>

#include "LevelChangeLogicBase.h"

class HeightLevelChangeLogic : public LevelChangeLogicBase
{
	ComponentDeclare(HeightLevelChangeLogic, LevelChangeLogicBase)

public:
	bool
	ShouldChangeLevel() override;

	void
	OnCreate() override;

	void
	SetHeightTarget(Next::Transform* a_heightTarget);

private:
	Next::Transform* m_heightTarget;

	Next::Transform* m_transform;
	Next::Transform* m_playerTransform;

	ReflectMembers(
		ReflectField(m_transform)
		ReflectField(m_heightTarget)
		ReflectField(m_playerTransform)
	)
};
