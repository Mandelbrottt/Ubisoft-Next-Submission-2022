#pragma once

#include <MinimalInclude.h>

class ControlsUi : public Next::Behaviour
{
	ComponentDeclare(ControlsUi, Next::Behaviour)

public:
	void
	OnUpdate() override;
};
