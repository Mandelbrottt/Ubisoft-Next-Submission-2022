#pragma once

#include <MinimalInclude.h>

class LevelChangeLogicBase : public Next::Behaviour
{
	ComponentDeclare(LevelChangeLogicBase, Behaviour)

public:
	virtual
	bool
	ShouldChangeLevel() = 0;
	
private:
	//ReflectMembers(
	//	ReflectField()
	//)
};
