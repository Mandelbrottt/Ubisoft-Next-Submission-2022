#pragma once

#include <MinimalInclude.h>

class WinScreen : public Next::Behaviour
{
	ComponentDeclare(WinScreen, Next::Behaviour)

public:
	void OnFirstUpdate() override;

	void OnUpdate() override;

private:
	bool m_enemiesExist = false;

	//ReflectMembers(
	//	ReflectField()
	//)
};
