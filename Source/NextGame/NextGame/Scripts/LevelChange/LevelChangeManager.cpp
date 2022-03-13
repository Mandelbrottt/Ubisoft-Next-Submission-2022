#include "LevelChangeManager.h"

#include "LevelChangeLogicBase.h"

ReflectRegister(LevelChangeManager);

using namespace Next;

void
LevelChangeManager::Init(
	LevelChangeLogicBase*   a_levelChangeLogic,
	Reflection::Type const& a_levelToChangeTo
)
{
	m_levelChangeLogicEntity = a_levelChangeLogic->GetEntity();
	m_levelChangeLogicType   = a_levelChangeLogic->GetType().GetTypeId();
	m_levelToChangeTo        = a_levelToChangeTo.GetTypeId();

	auto const& logicType = a_levelChangeLogic->GetType();
	auto const& sceneType = a_levelToChangeTo;
	
	assert(logicType.IsConvertibleTo(Reflection::Type::Get<LevelChangeLogicBase>()));
	assert(sceneType.IsConvertibleTo(Reflection::Type::Get<Scene>()));
}

void
LevelChangeManager::OnUpdate()
{
	auto levelChangeLogic = 
		static_cast<LevelChangeLogicBase*>(m_levelChangeLogicEntity.GetComponent(m_levelChangeLogicType));

	if (levelChangeLogic->ShouldChangeLevel())
	{
		SceneManager::LoadScene(m_levelToChangeTo);
	}
}

