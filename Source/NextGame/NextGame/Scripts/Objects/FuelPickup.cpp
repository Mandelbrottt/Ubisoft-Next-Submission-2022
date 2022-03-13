#include "FuelPickup.h"

#include "Scripts/Character/Player/ShipController.h"

ReflectRegister(FuelPickup);

using namespace Next;

void
FuelPickup::OnCreate()
{
	Entity modelEntity = Entity::Create(GetEntity().GetName() + " Model");

	auto modelRenderer = modelEntity.AddComponent<ModelRenderer>();
	modelRenderer->model = Model::Create("objects/fuel.obj");

	auto transform = modelEntity.Transform();

	//transform->SetLocalPosition({ 0, 1, 0 });
	transform->SetLocalScale({ 0.5f, 0.5f, 0.5f });
	transform->SetParent(Transform());
}

void
FuelPickup::OnUpdate()
{
	auto position = Transform()->GetPosition();
	if (position.y < ShipController::min_y)
	{
		position.y = ShipController::min_y;
		Transform()->SetPosition(position);
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
