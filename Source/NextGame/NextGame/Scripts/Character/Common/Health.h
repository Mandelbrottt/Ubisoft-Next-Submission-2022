#pragma once

#include <MinimalInclude.h>

class Health : public Next::Behaviour
{
	ComponentDeclare(Health, Next::Behaviour)

public:
	float
	GetHealth() const
	{
		return m_health;
	}

	void
	SubtractHealth();

	void
	Kill();
	
private:
	int m_health;

	ReflectMembers(
		ReflectField(m_health)
	)
};
