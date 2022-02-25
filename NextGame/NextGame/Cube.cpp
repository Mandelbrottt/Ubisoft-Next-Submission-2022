#include "Cube.h"

#include <MinimalInclude.h>

using namespace Next;

void
Cube::OnCreate()
{
	auto* transform       = Transform();
	transform->Position() = { 0.0f, 0.0f, 5 };
}

void
Cube::OnUpdate()
{
	//m_timeElapsed += Time::DeltaTime();

	auto& position = Transform()->Position();
	auto& rotation = Transform()->Rotation();

	float moveSpeed = 3;
	float turnSpeed = 3;
	
	position.x += Input::GetAxis(AxisCode::Horizontal)   * moveSpeed * Time::DeltaTime();
	position.y += Input::GetAxis(AxisCode::RightTrigger) * moveSpeed * Time::DeltaTime();
	position.y -= Input::GetAxis(AxisCode::LeftTrigger)  * moveSpeed * Time::DeltaTime();
	position.z += Input::GetAxis(AxisCode::Vertical)     * moveSpeed * Time::DeltaTime();

	rotation.x += Input::GetAxis(AxisCode::VerticalLook)   * turnSpeed * Time::DeltaTime();
	rotation.y += Input::GetAxis(AxisCode::HorizontalLook) * turnSpeed * Time::DeltaTime();
}
