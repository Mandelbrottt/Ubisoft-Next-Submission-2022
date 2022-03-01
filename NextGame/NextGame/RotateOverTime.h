#pragma once

#include "Scripting/Behaviour.h"

class RotateOverTime : public Next::Behaviour
{
	ReflectDeclare(RotateOverTime, Behaviour)

public:
	void OnCreate() override;
	void OnUpdate() override;

private:
	float m_theta = 0;

	float m_turnSpeed;
	
	ReflectMembers(
		ReflectField(m_theta)
		ReflectField(m_turnSpeed)
	)
};