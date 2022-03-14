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
	
	transform->SetLocalScale({ 0.5f, 0.5f, 0.5f });

	auto collider    = modelRenderer->AddComponent<SphereCollider>();
	collider->radius = 1;

	AddComponent<AccelerationBasedMover>();
	
	m_fuelAmount = Random::Value() * 10 + 5;

	m_gravityCalculator = AddComponent<GravityCalculator>();

	m_transform = Transform();
}

void
FuelPickup::OnUpdate()
{
	m_localGravity = m_gravityCalculator->CalculateGravity(PlayerShipController::flatPlanet);

	if (m_accelerationTarget)
	{
		auto    position  = Transform()->GetPosition();
		Vector3 direction = m_accelerationTarget->GetPosition() - position;
		direction.Normalize();

		m_velocity = direction * 15.f;

		Transform()->SetPosition(position + m_velocity * Time::DeltaTime());
	} else
	{
		auto mover = GetComponent<AccelerationBasedMover>();
		mover->ApplyAcceleration(m_localGravity + -m_velocity * 5.f);
		mover->Move();
	}

	// Calculate the amount by which to rotate to stabilize the ship
	Vector3 relativeUp    = m_transform->Up();
	Vector3 worldUp       = Vector::Normalize(-m_localGravity);

	float angle = Vector::Angle(relativeUp, worldUp);

	// Don't need to normalize result because Matrix::Rotate normalizes for us
	Vector3 rotationAxis = Vector::Cross(relativeUp, worldUp);
	
	if (rotationAxis != Vector3::Zero())
	{
		auto rotation = m_transform->GetLocalRotation();
		rotation *= Matrix::Rotate(angle, rotationAxis);
		m_transform->SetLocalRotation(rotation);
	}
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
