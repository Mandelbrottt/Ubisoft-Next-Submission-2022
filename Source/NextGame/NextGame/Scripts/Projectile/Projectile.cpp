#include "Projectile.h"

ReflectRegister(Projectile);

using namespace Next;

void
Projectile::Init(Vector3 a_direction, float a_speed, float a_lifespan, bool a_isFromEnemy)
{
	m_direction   = a_direction;
	m_speed       = a_speed;
	m_lifespan    = a_lifespan;
	m_isFromEnemy = a_isFromEnemy;
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
