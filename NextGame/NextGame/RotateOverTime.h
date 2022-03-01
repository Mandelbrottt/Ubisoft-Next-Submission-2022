#pragma once

#include "Scripting/Behaviour.h"

class RotateOverTime : public Next::Behaviour
{
	ReflectDeclare(RotateOverTime, Behaviour)

public:
	void OnUpdate() override;

private:
	float m_theta = 0;
	
	ReflectMembers(
		ReflectField(m_theta)
	)
};