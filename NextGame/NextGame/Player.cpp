#include "pch.h"

#include "Player.h"

#include <Application/Application.h>

using namespace NextCore;

enum
{
	AnimUp,
	AnimDown,
	AnimLeft,
	AnimRight,
};

void
Player::OnCreate()
{
	using NextCore::Sprite;

	// TEMPORARY:
	m_filePath       = Application::ResourcePath() + "Test.bmp";
	m_animationSpeed = 1.0f / 15.0f;

	auto* sprite = AddComponent<Sprite>();
	sprite->LoadFromTexture(m_filePath, 8, 4);

	sprite->SetPosition(400.0f, 400.0f);

	sprite->CreateAnimation(AnimDown, m_animationSpeed, { 0, 1, 2, 3, 4, 5, 6, 7 });
	sprite->CreateAnimation(AnimLeft, m_animationSpeed, { 8, 9, 10, 11, 12, 13, 14, 15 });
	sprite->CreateAnimation(AnimRight, m_animationSpeed, { 16, 17, 18, 19, 20, 21, 22, 23 });
	sprite->CreateAnimation(AnimUp, m_animationSpeed, { 24, 25, 26, 27, 28, 29, 30, 31 });

	sprite->SetScale(2.0f);
}

void
Player::OnUpdate()
{
	using NextCore::Sprite;
	using namespace NextCore::Math;

	Sprite* sprite = GetComponent<Sprite>();

	Vector2 position = sprite->GetPosition();

	if (Input::GetAxis(Input::Axis::LeftStickX) > 0.5f)
	{
		sprite->SetAnimation(AnimRight);
		position.x += 1.0f;
	}
	if (Input::GetAxis(Input::Axis::LeftStickX) < -0.5f)
	{
		sprite->SetAnimation(AnimLeft);
		position.x -= 1.0f;
	}
	if (Input::GetAxis(Input::Axis::LeftStickY) > 0.5f)
	{
		sprite->SetAnimation(AnimUp);
		position.y += 1.0f;
	}
	if (Input::GetAxis(Input::Axis::LeftStickY) < -0.5f)
	{
		sprite->SetAnimation(AnimDown);
		position.y -= 1.0f;
	}

	sprite->SetPosition(position);

	if (Input::GetButton(Input::Button::DPadUp) || GetAxis(Input::Axis::VerticalLook) > 0.5)
	{
		sprite->SetScale(sprite->GetScale() + 0.1f);
	}
	if (Input::GetButton(Input::Button::DPadDown) || GetAxis(Input::Axis::VerticalLook) < -0.5)
	{
		sprite->SetScale(sprite->GetScale() - 0.1f);
	}
	if (Input::GetButton(Input::Button::DPadLeft) || GetAxis(Input::Axis::HorizontalLook) < -0.5)
	{
		sprite->SetAngle(sprite->GetAngle() + 0.1f);
	}
	if (Input::GetButton(Input::Button::DPadRight) || GetAxis(Input::Axis::HorizontalLook) > 0.5)
	{
		sprite->SetAngle(sprite->GetAngle() - 0.1f);
	}
	if (Input::GetButtonDown(Input::Button::A)) // South Button
	{
		sprite->SetAnimation(-1);
	}
	if (Input::GetButtonDown(Input::Button::B)) // West Button
	{
		sprite->SetVertex(0, sprite->GetVertex(0) + Vector2(5.0f, 0));
	}
}
