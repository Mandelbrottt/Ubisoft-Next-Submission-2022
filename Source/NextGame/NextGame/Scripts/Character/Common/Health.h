#pragma once

#include <MinimalInclude.h>

class Health : public Next::Behaviour
{
	ComponentDeclare(Health, Next::Behaviour)

public:
	void
	DestroyOnKill(bool a_value)
	{
		m_destroyOnKill = a_value;
	}

	float
	GetHealth() const
	{
		return m_health;
	}

	void
	SetHealth(float a_newHealth)
	{
		m_health = a_newHealth;
	}

	bool
	IsDead() const
	{
		return m_health <= 0;
	}

	void
	SubtractHealth(float a_healthToSubtract = 1);

	void
	Kill();

private:
	float m_health        = 1;
	bool  m_destroyOnKill = true;

	ReflectMembers(
		ReflectField(m_health)
	)
};
