#pragma once

#include <MinimalInclude.h>

#include "LevelChangeLogicBase.h"

class DistanceLevelChangeLogic : public LevelChangeLogicBase
{
	ComponentDeclare(DistanceLevelChangeLogic, LevelChangeLogicBase)

	void
	OnFirstUpdate() override;
	
	bool
	ShouldChangeLevel() override;

public:
	float distanceToLeave = 150;

private:
	Next::Transform* m_playerTransform = nullptr;
	
	ReflectMembers(
		ReflectField(m_playerTransform)
	)
};
