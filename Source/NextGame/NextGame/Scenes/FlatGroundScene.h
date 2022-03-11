#pragma once

#include <MinimalInclude.h>

class FlatGroundScene : public Next::Scene
{
	ReflectDeclare(FlatGroundScene, Next::Scene)

public:
	void OnSceneCreate() override;
};
