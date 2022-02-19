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

	auto& rotation = Transform()->Rotation();
	
	if (Input::GetAxis(Input::Axis::Vertical) > 0.5)
	{
		rotation.x += 1.0f * Time::DeltaTime();
	}
	if (Input::GetAxis(Input::Axis::Vertical) < -0.5)
	{
		rotation.x -= 1.0f * Time::DeltaTime();
	}
	if (Input::GetAxis(Input::Axis::Horizontal) > 0.5)
	{
		rotation.y += 1.0f * Time::DeltaTime();
	}
	if (Input::GetAxis(Input::Axis::Horizontal) < -0.5)
	{
		rotation.y -= 1.0f * Time::DeltaTime();
	}
	
	//Transform()->Rotation() = { m_timeElapsed * 0.5f, m_timeElapsed };
}
