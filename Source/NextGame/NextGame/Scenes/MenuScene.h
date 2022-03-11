#pragma once

#include <HeaderInclude.h>

class MenuScene : public Next::Scene
{
	ReflectDeclare(MenuScene, Scene)

public:
	void
	OnSceneCreate() override;
};
