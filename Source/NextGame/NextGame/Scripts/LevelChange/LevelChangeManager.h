#pragma once

#include <Scripting/Behaviour.h>

class LevelChangeManager : public Next::Behaviour
{
	ComponentDeclare(LevelChangeManager, Behaviour)

public:
	// Must be called before OnFirstUpdate()
	void
	Init(Next::Entity a_levelChangeLogicEntity, Next::Reflection::TypeId a_levelChangeLogicType);

	void
	OnFirstUpdate() override;
	
private:
	// Store the entity and the type because NextCore doesn't currently support polymorphic reference tracking
	Next::Entity             m_levelChangeLogicEntity;
	Next::Reflection::TypeId m_levelChangeLogicType;
};
