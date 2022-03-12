#include "Health.h"

ReflectRegister(Health);

void
Health::SubtractHealth(float a_healthToSubtract)
{
	m_health -= a_healthToSubtract;

	if (m_health <= 0)
	{
		m_health = 0;
		
		if (m_destroyOnKill)
		{
			Kill();
		}
	}
}

void
Health::Kill()
{
	GetEntity().Destroy();
}
