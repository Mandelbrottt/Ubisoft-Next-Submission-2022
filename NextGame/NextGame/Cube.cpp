#include "Cube.h"

#include <MinimalInclude.h>

#include <NextAPI/app.h>

using namespace NextCore;
using Graphics::Sprite;
using namespace Math;

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
	
	position.x += GetAxis(Input::Axis::Horizontal)   * moveSpeed * Time::DeltaTime();
	position.y += GetAxis(Input::Axis::RightTrigger) * moveSpeed * Time::DeltaTime();
	position.y -= GetAxis(Input::Axis::LeftTrigger)  * moveSpeed * Time::DeltaTime();
	position.z += GetAxis(Input::Axis::Vertical)     * moveSpeed * Time::DeltaTime();

	rotation.x += GetAxis(Input::Axis::VerticalLook)   * turnSpeed * Time::DeltaTime();
	rotation.y += GetAxis(Input::Axis::HorizontalLook) * turnSpeed * Time::DeltaTime();
}
