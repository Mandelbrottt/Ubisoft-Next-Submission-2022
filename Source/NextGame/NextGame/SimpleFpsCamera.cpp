#include "SimpleFpsCamera.h"

#include <MinimalInclude.h>
#include <algorithm>

using namespace Next;

ReflectRegister(SimpleFpsCamera);

void
SimpleFpsCamera::OnCreate()
{
	auto* camera = AddComponent<Camera>();
	camera->SetFov(105, FovType::Horizontal);

	m_transform = Transform();

	auto* audioSource = AddComponent<AudioSource>();
	audioSource->audioClip = AudioClip::Create();
}

void
SimpleFpsCamera::OnUpdate()
{
	auto position = m_transform->GetPosition();
	auto rotation = m_transform->GetRotation();

	float moveSpeed = 8;
	float turnSpeed = 180;

	position += m_transform->Forward() * Input::GetAxis(Axis::Vertical)     * moveSpeed * Time::DeltaTime();
	position += m_transform->Right()   * Input::GetAxis(Axis::Horizontal)   * moveSpeed * Time::DeltaTime();
	position += m_transform->Up()      * Input::GetAxis(Axis::RightTrigger) * moveSpeed * Time::DeltaTime();
	position -= m_transform->Up()      * Input::GetAxis(Axis::LeftTrigger)  * moveSpeed * Time::DeltaTime();
	
	rotation.x += Input::GetAxis(Axis::VerticalLook)   * turnSpeed * Time::DeltaTime();
	rotation.y += Input::GetAxis(Axis::HorizontalLook) * turnSpeed * Time::DeltaTime();

	rotation.x = std::clamp(rotation.x, -85.f, 85.f);

	m_transform->SetPosition(position);
	m_transform->SetRotation(rotation);
}
