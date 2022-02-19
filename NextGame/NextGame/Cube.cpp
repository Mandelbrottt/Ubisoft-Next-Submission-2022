#include "Cube.h"

#include <MinimalInclude.h>

using namespace NextCore;
using Graphics::Sprite;
using namespace Math;

void
Cube::OnCreate()
{
	// Create the sound
	m_baseFilePath = Application::ResourcePath() + "cube/";

	{ // 1
		auto* sprite     = AddComponent<Sprite>();
		sprite->LoadFromTexture(m_baseFilePath + "black.bmp");
		sprite->SetVertex(0, {  1,  1, -1 }); //1.000000 1.000000 -1.000000
		sprite->SetVertex(1, { -1,  1, -1 }); //-1.000000 1.000000 -1.000000
		sprite->SetVertex(2, { -1,  1,  1 }); //-1.000000 1.000000 1.000000
		sprite->SetVertex(3, {  1,  1,  1 }); //1.000000 1.000000 1.000000
	}
	{ // 2
		auto* sprite     = AddComponent<Sprite>();
		sprite->LoadFromTexture(m_baseFilePath + "white.bmp");
		sprite->SetVertex(0, {  1, -1,  1 }); //1.000000 -1.000000 1.000000
		sprite->SetVertex(1, {  1,  1,  1 }); //1.000000 1.000000 1.000000
		sprite->SetVertex(2, { -1,  1,  1 }); //-1.000000 1.000000 1.000000
		sprite->SetVertex(3, { -1, -1,  1 }); //-1.000000 -1.000000 1.000000
	}
	{ // 3
		auto* sprite     = AddComponent<Sprite>();
		sprite->LoadFromTexture(m_baseFilePath + "red.bmp");
		sprite->SetVertex(0, { -1, -1,  1 }); //-1.000000 -1.000000 1.000000
		sprite->SetVertex(1, { -1,  1,  1 }); //-1.000000 1.000000 1.000000
		sprite->SetVertex(2, { -1,  1, -1 }); //-1.000000 1.000000 -1.000000
		sprite->SetVertex(3, { -1, -1, -1 }); //-1.000000 -1.000000 -1.000000
	}
	{ // 4
		auto* sprite     = AddComponent<Sprite>();
		sprite->LoadFromTexture(m_baseFilePath + "blue.bmp");
		sprite->SetVertex(0, { -1, -1, -1 }); //-1.000000 -1.000000 -1.000000
		sprite->SetVertex(1, {  1, -1, -1 }); //1.000000 -1.000000 -1.000000
		sprite->SetVertex(2, {  1, -1,  1 }); //1.000000 -1.000000 1.000000
		sprite->SetVertex(3, { -1, -1,  1 }); //-1.000000 -1.000000 1.000000
	}
	{ // 5
		auto* sprite     = AddComponent<Sprite>();
		sprite->LoadFromTexture(m_baseFilePath + "green.bmp");
		sprite->SetVertex(0, {  1, -1, -1 }); //1.000000 -1.000000 -1.000000
		sprite->SetVertex(1, {  1,  1, -1 }); //1.000000 1.000000 -1.000000
		sprite->SetVertex(2, {  1,  1,  1 }); //1.000000 1.000000 1.000000
		sprite->SetVertex(3, {  1, -1,  1 }); //1.000000 -1.000000 1.000000
	}
	
	{ // 6
		auto* sprite     = AddComponent<Sprite>();
		sprite->LoadFromTexture(m_baseFilePath + "yellow.bmp");
		sprite->SetVertex(0, { -1, -1, -1 }); //-1.000000 -1.000000 -1.000000
		sprite->SetVertex(1, { -1,  1, -1 }); //-1.000000 1.000000 -1.000000
		sprite->SetVertex(2, {  1,  1, -1 }); //1.000000 1.000000 -1.000000
		sprite->SetVertex(3, {  1, -1, -1 }); //1.000000 -1.000000 -1.000000
	}

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
