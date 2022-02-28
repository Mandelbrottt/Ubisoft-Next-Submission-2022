#include "Cube.h"

#include <MinimalInclude.h>

using namespace Next;

void
Cube::OnCreate()
{
	auto* transform = Transform();

	transform->SetPosition({ 0.0f, 0.0f, 0 });

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
	float turnSpeed = 3;

	position += transform->Forward() * Input::GetAxis(AxisCode::Vertical)     * moveSpeed * Time::DeltaTime();
	position += transform->Right()   * Input::GetAxis(AxisCode::Horizontal)   * moveSpeed * Time::DeltaTime();
	position += transform->Up()      * Input::GetAxis(AxisCode::RightTrigger) * moveSpeed * Time::DeltaTime();
	position -= transform->Up()      * Input::GetAxis(AxisCode::LeftTrigger)  * moveSpeed * Time::DeltaTime();
	
	rotation.x += Input::GetAxis(AxisCode::VerticalLook)   * turnSpeed * Time::DeltaTime();
	rotation.y -= Input::GetAxis(AxisCode::HorizontalLook) * turnSpeed * Time::DeltaTime();

	transform->SetPosition(position);
	transform->SetRotation(rotation);
}
