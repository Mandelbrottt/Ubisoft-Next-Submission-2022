#pragma once

#include <MinimalInclude.h>

#include <Components/AudioSource.h>
#include <Components/AudioSource.h>

class Projectile : public Next::Behaviour
{
	ComponentDeclare(Projectile, Next::Behaviour)

public:
	void
	Init(Next::Vector3 a_direction, float a_speed, float a_lifespan, bool a_isFromEnemy);

	void
	OnFirstUpdate() override;

	void
	OnUpdate() override;

	void
	OnTriggerCollisionStart(Next::Collider* a_other) override;

	void
	OnTriggerCollision(Next::Collider* a_other) override;

	void
	OnTriggerCollisionEnd(Next::Collider* a_other) override;

private:
	Next::Vector3 m_direction;

	float m_speed;

	bool m_isFromEnemy;

	float m_lifespan = 0;
	float m_timer    = 0;
};
