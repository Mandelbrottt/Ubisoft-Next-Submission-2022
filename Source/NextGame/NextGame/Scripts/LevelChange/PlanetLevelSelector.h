#pragma once

#include <MinimalInclude.h>

class PlanetLevelSelector : public Next::Behaviour
{
	ComponentDeclare(PlanetLevelSelector, Next::Behaviour)

public:
	void
	OnCreate() override;

	void
	Init(
		std::string_view const&       a_planetFile,
		float                         a_radius,
		float                         a_spinSpeed,
		Next::Reflection::Type const& a_sceneType
	);

	void
	OnTriggerCollisionStart(Next::Collider* a_other) override;

private:
	Next::Reflection::TypeId m_sceneTypeId;

	//ReflectMembers(
	//	ReflectField()
	//)
};
