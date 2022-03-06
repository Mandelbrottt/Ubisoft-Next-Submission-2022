#include "Cube.h"

#include <MinimalInclude.h>
#include <algorithm>

using namespace Next;

ReflectRegister(Cube);

void
Cube::OnCreate()
{
	auto* transform = Transform();

	transform->SetPosition({ 10, 10, -10 });
	transform->SetRotation({ -45, -45, 0 });

	auto* camera = AddComponent<Camera>();
	camera->SetFov(105, FovType::Horizontal);
}

void
Cube::OnUpdate()
{
	auto transform = Transform();
	
	auto position = transform->GetPosition();
	auto rotation = transform->GetRotation();

	float moveSpeed = 3;
	float turnSpeed = 180;

	position += transform->Forward() * Input::GetAxis(Axis::Vertical)     * moveSpeed * Time::DeltaTime();
	position += transform->Right()   * Input::GetAxis(Axis::Horizontal)   * moveSpeed * Time::DeltaTime();
	position += transform->Up()      * Input::GetAxis(Axis::RightTrigger) * moveSpeed * Time::DeltaTime();
	position -= transform->Up()      * Input::GetAxis(Axis::LeftTrigger)  * moveSpeed * Time::DeltaTime();
	
	rotation.x += Input::GetAxis(Axis::VerticalLook)   * turnSpeed * Time::DeltaTime();
	rotation.y += Input::GetAxis(Axis::HorizontalLook) * turnSpeed * Time::DeltaTime();

	rotation.x = std::clamp(rotation.x, -85.f, 85.f);

	transform->SetPosition(position);
	transform->SetRotation(rotation);
}
