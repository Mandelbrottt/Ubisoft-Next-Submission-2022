#pragma once

#include <Scenes/Scene.h>

class SceneA : public Next::Scene
{
	ReflectDeclare(SceneA, Scene)

	void
	OnSceneCreate() override;
};

class SceneB : public Next::Scene
{
	ReflectDeclare(SceneB, Scene)

	void
	OnSceneCreate() override;
};
