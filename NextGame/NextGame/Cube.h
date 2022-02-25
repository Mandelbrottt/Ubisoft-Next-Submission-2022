#pragma once

#include <Scripting/Behaviour.h>

class Cube : public Next::Behaviour
{
	GenerateConstructors(Cube)
public:
	void OnCreate() override;

	void OnUpdate() override;

private:
	float m_timeElapsed = 0;
	
	std::string m_baseFilePath;

	REFLECT_DECLARE(Cube, Behaviour)

	REFLECT_MEMBERS(
		REFLECT_FIELD(m_baseFilePath)
	)
};
