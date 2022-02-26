#pragma once

#include "Scripting/Behaviour.h"

class RotateOverTime : public Next::Behaviour
{
	GenerateConstructors(RotateOverTime)
public:
	void OnUpdate() override;

private:
	float m_theta = 0;

	REFLECT_DECLARE(RotateOverTime, Behaviour)

	REFLECT_MEMBERS(
		REFLECT_FIELD(m_theta)
	)
};