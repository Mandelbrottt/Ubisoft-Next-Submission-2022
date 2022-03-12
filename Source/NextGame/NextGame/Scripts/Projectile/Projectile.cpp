#include "Projectile.h"

#include <Components/Colliders/SphereCollider.h>

#include "Scripts/Character/Common/Health.h"
#include "Scripts/Character/Enemy/TurretFireController.h"
#include "Scripts/Character/Player/PlayerShip.h"

ReflectRegister(Projectile);

using namespace Next;

void
Projectile::Init(Vector3 a_direction, float a_speed, float a_lifespan, bool a_isFromEnemy)
{
	m_direction   = a_direction;
	m_speed       = a_speed;
	m_lifespan    = a_lifespan;
	m_isFromEnemy = a_isFromEnemy;

	auto collider = AddComponent<SphereCollider>();
	collider->radius = 1;
}

void
Projectile::OnFirstUpdate()
{
	std::string modelName = m_isFromEnemy ? "redProjectile.obj" : "blueProjectile.obj";

	auto modelRenderer   = AddComponent<ModelRenderer>();
	modelRenderer->model = Model::Create("objects/" + modelName);
}

void
Projectile::OnUpdate()
{
	auto transform = Transform();

	auto position = transform->GetPosition();

	position += m_direction * m_speed * Time::DeltaTime();

	transform->SetPosition(position);

	m_timer += Time::DeltaTime();
	if (m_timer > m_lifespan)
	{
		GetEntity().Destroy();
	}
}

void
Projectile::OnTriggerCollisionStart(Collider* a_other)
{
	using Reflection::TypeId;
	TypeId typeToCheckFor;
	
	if (m_isFromEnemy)
	{
		typeToCheckFor = Reflection::GetTypeId<PlayerShip>();
	} else
	{
		typeToCheckFor = Reflection::GetTypeId<TurretFireController>();
	}

	auto otherComponent = a_other->GetComponent(typeToCheckFor);
	if (!otherComponent)
	{
		return;
	}

	auto health = otherComponent->GetComponent<Health>();
	if (!health)
	{
		return;
	}
	
	health->SubtractHealth();
}