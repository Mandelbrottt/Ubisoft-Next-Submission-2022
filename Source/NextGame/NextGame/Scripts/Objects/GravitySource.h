#pragma once

#include <MinimalInclude.h>

class GravitySource : public Next::Behaviour
{
	ComponentDeclare(GravitySource, Next::Behaviour)

public:
	float gravityStrength = 1;

private:
	ReflectMembers(
		ReflectField(gravityStrength)
	)
};
