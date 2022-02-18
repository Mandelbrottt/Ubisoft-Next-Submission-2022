#include "Player.h"

#include <Application/Application.h>

#include <Components/AudioSource.h>
#include <Components/Transform.h>

REFLECT_REGISTER(Player);

using namespace NextCore;

using namespace Component;

enum
{
	AnimUp,
	AnimDown,
	AnimLeft,
	AnimRight,
};

using Graphics::Sprite;
using namespace Math;

void
Player::OnCreate()
{
	// Create the sound
	std::string path        = Application::ResourcePath() + "Test.wav";
	auto*       audioSource = AddComponent<AudioSource>();
	audioSource->Load(path);

	// Create the sprite
	m_filePath       = Application::ResourcePath() + "Test.bmp";
	m_animationSpeed = 1.0f / 15.0f;
	auto* sprite     = AddComponent<Sprite>();
	sprite->LoadFromTexture(m_filePath, 8, 4);

	auto* transform       = Transform();
	transform->Position() = { 0.0f, 0.0f };
	transform->Scale()    = Vector3(2.0f);

	// Sprite animations
	sprite->CreateAnimation(AnimDown, m_animationSpeed, { 0, 1, 2, 3, 4, 5, 6, 7 });
	sprite->CreateAnimation(AnimLeft, m_animationSpeed, { 8, 9, 10, 11, 12, 13, 14, 15 });
	sprite->CreateAnimation(AnimRight, m_animationSpeed, { 16, 17, 18, 19, 20, 21, 22, 23 });
	sprite->CreateAnimation(AnimUp, m_animationSpeed, { 24, 25, 26, 27, 28, 29, 30, 31 });
}

void
Player::OnUpdate()
{
	Sprite* sprite = GetComponent<Sprite>();

	auto* transform = Transform();
	auto& position  = transform->Position();

	// Movement
	if (Input::GetAxis(Input::Axis::LeftStickX) > 0.5f)
	{
		sprite->SetAnimation(AnimRight);
		position.x += 1.0f * Time::DeltaTime();
	}
	if (Input::GetAxis(Input::Axis::LeftStickX) < -0.5f)
	{
		sprite->SetAnimation(AnimLeft);
		position.x -= 1.0f * Time::DeltaTime();
	}
	if (Input::GetAxis(Input::Axis::LeftStickY) > 0.5f)
	{
		sprite->SetAnimation(AnimUp);
		position.y += 1.0f * Time::DeltaTime();
	}
	if (Input::GetAxis(Input::Axis::LeftStickY) < -0.5f)
	{
		sprite->SetAnimation(AnimDown);
		position.y -= 1.0f * Time::DeltaTime();
	}

	auto& scale = transform->Scale().z;
	auto& angle = transform->Rotation().z;

	// Sprite rotation and scale
	if (Input::GetButton(Input::Button::DPadUp) || GetAxis(Input::Axis::VerticalLook) > 0.5)
	{
		scale += 0.1f;
	}
	if (Input::GetButton(Input::Button::DPadDown) || GetAxis(Input::Axis::VerticalLook) < -0.5)
	{
		scale -= 0.1f;
	}
	if (Input::GetButton(Input::Button::DPadLeft) || GetAxis(Input::Axis::HorizontalLook) < -0.5)
	{
		angle += 0.1f;
	}
	if (Input::GetButton(Input::Button::DPadRight) || GetAxis(Input::Axis::HorizontalLook) > 0.5)
	{
		angle -= 0.1f;
	}
	if (Input::GetButtonDown(Input::Button::A)) // South Button
	{
		sprite->SetAnimation(-1);
	}
	//if (Input::GetButtonDown(Input::Button::B)) // West Button
	//{
	//	sprite->SetVertex(0, sprite->GetVertex(0) + Vector2(5.0f, 0));
	//}

	// Sample Sound
	if (Input::GetButtonDown(Input::Button::B))
	{
		auto* audioSource = GetComponent<AudioSource>();
		audioSource->Play();
	}
}
