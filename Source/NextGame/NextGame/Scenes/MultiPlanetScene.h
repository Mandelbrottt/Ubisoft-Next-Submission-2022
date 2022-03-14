#pragma once

#include <MinimalInclude.h>

class MultiPlanetScene : public Next::Scene
{
	ReflectDeclare(MultiPlanetScene, Next::Scene)

public:
	void
	OnSceneCreate() override;

	void
	CreateEnvironment();

	void
	CreatePlanet(
		std::string_view const& a_filename,
		Next::Vector3           a_position,
		float                   a_radius,
		float                   a_numTurrets,
		float                   a_numPickups
	);

private:
	//ReflectMembers(
	//	ReflectField()
	//)
};
