#pragma once

#include <MinimalInclude.h>

#include <NextAPI/app.h>

class Player final : public NextCore::Scripting::Behaviour
{
public:
	void
	OnCreate() override;

	void
	OnUpdate() override;

private:
	float m_animationSpeed = 0;

	std::string m_filePath;

	REFLECT_DECLARE(Player)

	REFLECT_MEMBERS(
		REFLECT_FIELD(m_animationSpeed, r_name = "Animation Speed")
	)
};
