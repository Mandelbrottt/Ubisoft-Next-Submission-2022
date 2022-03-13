#include "SpinInPlace.h"

ReflectRegister(SpinInPlace);

using namespace Next;

void
SpinInPlace::OnUpdate()
{
	m_yaw += spinSpeed * Time::DeltaTime();

	Transform()->SetLocalRotation({ 0, m_yaw, 0 });

	m_yaw = fmod(m_yaw, 360.f);
}