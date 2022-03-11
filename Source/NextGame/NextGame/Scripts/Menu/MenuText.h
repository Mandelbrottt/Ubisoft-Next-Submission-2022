#pragma once

#include <MinimalInclude.h>

class MenuText : public Next::Behaviour
{
	ComponentDeclare(MenuText, Behaviour)

public:
	void
	OnCreate() override;

	void
	OnUpdate() override;

private:
	std::string m_titleText;
	std::string m_startText;
};
