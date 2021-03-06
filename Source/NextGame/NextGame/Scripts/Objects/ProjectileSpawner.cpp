#include "ProjectileSpawner.h"

#include <Components/AudioSource.h>
#include <Components/AudioSource.h>
#include <Components/AudioSource.h>
#include <Components/AudioSource.h>

#include "Projectile.h"

ReflectRegister(ProjectileSpawner);

using namespace Next;

void
ProjectileSpawner::SpawnProjectile(Vector3 a_position, Vector3 a_direction)
{
	Entity projectileEntity = Entity::Create("Projectile");
	
	auto projectile = projectileEntity.AddComponent<Projectile>();
	
	projectile->Init(a_direction, projectileSpeed, 5, isEnemyOwned);
	projectile->Transform()->SetPosition(a_position);
}
