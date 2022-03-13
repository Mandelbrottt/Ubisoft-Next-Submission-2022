#include "FuelPickup.h"

#include "Scripts/Character/Player/PlayerShip.h"
#include "Scripts/Character/Player/PlayerShipController.h"

ReflectRegister(FuelPickup);

using namespace Next;

void
FuelPickup::OnCreate()
{
	auto modelRenderer   = AddComponent<ModelRenderer>();
	modelRenderer->model = Model::Create("objects/fuel.obj");

	auto transform = Transform();

	//transform->SetLocalPosition({ 0, 1, 0 });
	transform->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	auto collider = modelRenderer->AddComponent<SphereCollider>();
	collider->radius = 1;

	m_localGravity = PlayerShipController::gravity;

	m_fuelAmount = Random::Value() * 10 + 5;
}

void
FuelPickup::OnUpdate()
{
	auto position = Transform()->GetPosition();
	
	if (m_accelerationTarget)
	{
		Vector3 direction = m_accelerationTarget->GetPosition() - position;
		direction.Normalize();

		m_velocity = direction * 15.f;
	} else
	{
		Vector3 acceleration = m_localGravity + -m_velocity * 5.f;
		m_velocity += acceleration * Time::DeltaTime();
	}

	position += m_velocity * Time::DeltaTime();
	if (position.y < PlayerShipController::min_y)
	{
		m_velocity.y = 0;
		position.y = PlayerShipController::min_y;
	}
	Transform()->SetPosition(position);
}

float
FuelPickup::GetFuelAmount() const
{
	return m_fuelAmount;
}

void
FuelPickup::SetFuelAmount(float a_fuelAmount)
{
	m_fuelAmount = std::max(a_fuelAmount, 0.0f);
}

void
FuelPickup::SetAccelerationTarget(Next::Transform* a_target)
{
	m_accelerationTarget = a_target;
}

void
FuelPickup::OnTriggerCollisionStart(Collider* a_other)
{
	auto fuelController = a_other->GetComponent<PlayerFuelController>();

	if (!fuelController)
	{
		return;
	}

	// Destroys this fuel
	fuelController->ConsumeFuelPickup(this);
}
