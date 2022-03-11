#pragma once

#include <MinimalInclude.h>

class MenuLogic : public Next::Behaviour
{
	ComponentDeclare(MenuLogic, Next::Behaviour)

public:
	void OnUpdate() override;

private:
	//ReflectMembers(
	//	ReflectField()
	//)
};
