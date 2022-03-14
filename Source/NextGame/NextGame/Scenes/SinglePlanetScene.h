#pragma once

#include <MinimalInclude.h>

class SinglePlanetScene : public Next::Scene
{
	ReflectDeclare(SinglePlanetScene, Scene)

public:
	void
	OnSceneCreate() override;

	void
	CreateEnvironment(float a_radius);

	void
	CreateObjects(float a_radius);
};
