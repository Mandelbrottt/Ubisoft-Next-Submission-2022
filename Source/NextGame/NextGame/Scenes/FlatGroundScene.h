#pragma once

#include <MinimalInclude.h>

class FlatGroundScene : public Next::Scene
{
	ReflectDeclare(FlatGroundScene, Next::Scene)

public:
	void OnSceneCreate() override;
	void OnSceneDestroy() override;

private:
	void
	CreateEnvironment(float a_size);

	void
	CreateObjects(float a_size);
};
