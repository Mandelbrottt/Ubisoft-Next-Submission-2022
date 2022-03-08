#pragma once

#include "Scripting/Behaviour.h"

class RotateOverTime : public Next::Behaviour
{
	ReflectDeclare(RotateOverTime, Behaviour)

public:
	void OnCreate() override;
	void OnUpdate() override;

	void Init(int a_number)
	{
		m_turnSpeed = 50.f + 10.f * a_number;
	}

private:
	float m_theta = 0;

	float m_turnSpeed;
	
	ReflectMembers(
		ReflectField(m_theta)
		ReflectField(m_turnSpeed)
	)
};