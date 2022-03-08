#pragma once

#include <Scripting/Behaviour.h>

class Cube : public Next::Behaviour
{
	ReflectDeclare(Cube, Behaviour)

public:
	void OnCreate() override;

	void OnUpdate() override;

private:
	float m_timeElapsed = 0;
	
	std::string m_baseFilePath;
	
	ReflectMembers(
		ReflectField(m_baseFilePath)
	)
};
