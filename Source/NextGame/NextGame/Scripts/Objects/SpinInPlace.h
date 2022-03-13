#pragma once

#include <MinimalInclude.h>

class SpinInPlace : public Next::Behaviour
{
	ComponentDeclare(SpinInPlace, Next::Behaviour)

public:
	void
	OnUpdate() override;
	
	float spinSpeed = 90;

private:
	float m_yaw = 0;
	
	ReflectMembers(
		ReflectField(spinSpeed)
	)
};
