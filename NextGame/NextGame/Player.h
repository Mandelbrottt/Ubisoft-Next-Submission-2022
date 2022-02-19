#pragma once

#include <MinimalInclude.h>

class Player final : public NextCore::Scripting::Behaviour
{
	GenerateConstructors(Player)

public:
	void
	OnCreate() override;

	void
	OnUpdate() override;

private:
	float m_animationSpeed = 0;

	std::string m_filePath;

	REFLECT_DECLARE(Player, Behaviour)

	REFLECT_MEMBERS(
		REFLECT_FIELD(m_animationSpeed, r_name = "Animation Speed")
	)
};