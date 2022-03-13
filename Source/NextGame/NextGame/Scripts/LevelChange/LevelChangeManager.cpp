#include "LevelChangeManager.h"

ReflectRegister(LevelChangeManager);

void
LevelChangeManager::Init(Next::Entity a_levelChangeLogicEntity, Next::Reflection::TypeId a_levelChangeLogicType)
{
	m_levelChangeLogicEntity = a_levelChangeLogicEntity;
	m_levelChangeLogicType   = a_levelChangeLogicType;
}

void
LevelChangeManager::OnFirstUpdate()
{
	
}
