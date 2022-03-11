#pragma once

#include <MinimalInclude.h>

class ProjectileSpawner : public Next::Behaviour
{
	ComponentDeclare(ProjectileSpawner, Next::Behaviour)

public:
	void
	SpawnProjectile(Next::Vector3 a_direction);
	
public:
	bool isEnemyOwned;
	float projectileSpeed;

private:
	//ReflectMembers(
	//	ReflectField()
	//)
};
