#pragma once

#include <Scripting/Behaviour.h>

class CrazyLineThing : public Next::Behaviour
{
	GenerateConstructors(CrazyLineThing)

	int a;

public:
	void
	OnCreate() override;
	
	void
	OnUpdate() override;
	
	REFLECT_DECLARE(CrazyLineThing, Behaviour)

	REFLECT_MEMBERS(
		REFLECT_FIELD(a)
	)
};