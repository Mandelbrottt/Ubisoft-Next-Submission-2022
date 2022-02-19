#include "Cube.h"

#include <MinimalInclude.h>

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
	
	position.x += GetAxis(Input::Axis::Horizontal)   * Time::DeltaTime();
	position.y += GetAxis(Input::Axis::RightTrigger) * Time::DeltaTime();
	position.y -= GetAxis(Input::Axis::LeftTrigger)  * Time::DeltaTime();
	position.z += GetAxis(Input::Axis::Vertical)     * Time::DeltaTime();

	rotation.x += GetAxis(Input::Axis::VerticalLook)   * Time::DeltaTime();
	rotation.y += GetAxis(Input::Axis::HorizontalLook) * Time::DeltaTime();
}
