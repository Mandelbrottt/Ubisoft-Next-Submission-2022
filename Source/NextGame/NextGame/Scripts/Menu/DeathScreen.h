#pragma once

#include <MinimalInclude.h>

#include "Scripts/Character/Common/Health.h"

class DeathScreen : public Next::Behaviour
{
	ComponentDeclare(DeathScreen, Next::Behaviour)

public:
	void
	OnFirstUpdate() override;

	void
	OnUpdate() override;

private:
	Health* m_playerHealth = nullptr;
		
	//ReflectMembers(
	//	ReflectField()
	//)
};
