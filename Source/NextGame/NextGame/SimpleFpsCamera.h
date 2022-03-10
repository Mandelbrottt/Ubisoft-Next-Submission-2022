#pragma once

#include <Scripting/Behaviour.h>

class SimpleFpsCamera : public Next::Behaviour
{
	ComponentDeclare(SimpleFpsCamera, Behaviour)

public:
	void OnCreate() override;
	void OnUpdate() override;

private:
	Next::Transform* m_transform;
	
	ReflectMembers(
		ReflectField(m_transform)
	)
};
