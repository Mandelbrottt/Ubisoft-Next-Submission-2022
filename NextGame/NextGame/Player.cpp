#include "pch.h"

#include "Player.h"

#include <Application/Application.h>

using namespace NextCore;

enum
{
	AnimForwards,
	AnimBackwards,
	AnimLeft,
	AnimRight,
};

void
Player::OnCreate()
{
	using NextCore::Sprite;

	// TEMPORARY:
	m_filePath = Application::ResourcePath() + "Test.bmp";
	m_animationSpeed = 1.0f / 15.0f;

	auto* sprite = AddComponent<Sprite>();
	sprite->LoadFromTexture(m_filePath, 8, 4);
	
	sprite->SetPosition(400.0f, 400.0f);
	
	sprite->CreateAnimation(AnimBackwards, m_animationSpeed, { 0, 1, 2, 3, 4, 5, 6, 7 });
	sprite->CreateAnimation(AnimLeft,      m_animationSpeed, { 8, 9, 10, 11, 12, 13, 14, 15 });
	sprite->CreateAnimation(AnimRight,     m_animationSpeed, { 16, 17, 18, 19, 20, 21, 22, 23 });
	sprite->CreateAnimation(AnimForwards,  m_animationSpeed, { 24, 25, 26, 27, 28, 29, 30, 31 });

	sprite->SetScale(2.0f);
}

void
Player::OnUpdate()
{
	using NextCore::Sprite;
	using namespace NextCore::Math;

	Sprite* sprite = GetComponent<Sprite>();

	Vector2 position = sprite->GetPosition();

	if (App::GetController().GetLeftThumbStickX() > 0.5f)
	{
		sprite->SetAnimation(AnimRight);
		position.x += 1.0f;
	}
	if (App::GetController().GetLeftThumbStickX() < -0.5f)
	{
		sprite->SetAnimation(AnimLeft);
		position.x -= 1.0f;
	}
	if (App::GetController().GetLeftThumbStickY() > 0.5f)
	{
		sprite->SetAnimation(AnimForwards);
		position.y -= 1.0f;
	}
	if (App::GetController().GetLeftThumbStickY() < -0.5f)
	{
		sprite->SetAnimation(AnimBackwards);
		position.y += 1.0f;
	}

	sprite->SetPosition(position);

	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
	{
		sprite->SetScale(sprite->GetScale() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
	{
		sprite->SetScale(sprite->GetScale() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
	{
		sprite->SetAngle(sprite->GetAngle() + 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
	{
		sprite->SetAngle(sprite->GetAngle() - 0.1f);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_A, true))
	{
		sprite->SetAnimation(-1);
	}
	if (App::GetController().CheckButton(XINPUT_GAMEPAD_B, true))
	{
		sprite->SetVertex(0, sprite->GetVertex(0) + Vector2(5.0f, 0));
	}
}
