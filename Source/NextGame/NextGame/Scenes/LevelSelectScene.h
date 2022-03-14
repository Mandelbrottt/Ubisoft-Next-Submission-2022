#pragma once

#include <MinimalInclude.h>

class LevelSelectScene : public Next::Scene
{
	ReflectDeclare(LevelSelectScene, Next::Scene)

public:
	void OnSceneCreate() override;
};
