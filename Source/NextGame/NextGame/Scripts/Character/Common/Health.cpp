#include "Health.h"

ReflectRegister(Health);

void
Health::SubtractHealth()
{
	m_health -= 1;

	if (m_health <= 0)
	{
		Kill();
	}
}

void
Health::Kill()
{
	GetEntity().Destroy();
}
