#pragma once

#include <Scripting/Behaviour.h>

#include "LevelChangeLogicBase.h"

class LevelChangeManager : public Next::Behaviour
{
	ComponentDeclare(LevelChangeManager, Behaviour)

public:
	// Must be called before OnFirstUpdate()
	void
	Init(
		LevelChangeLogicBase*         a_levelChangeLogic,
		Next::Reflection::Type const& a_levelToChangeTo
	);
	
	void
	OnUpdate() override;
	
private:
	// Store the entity and the type because NextCore doesn't currently support polymorphic reference tracking
	Next::Entity             m_levelChangeLogicEntity;
	Next::Reflection::TypeId m_levelChangeLogicType;

	Next::Reflection::TypeId m_levelToChangeTo;
};
