#include "MenuScene.h"

#include <MinimalInclude.h>

#include "Scripts/Character/Player/PlayerPersistentData.h"
#include "Scripts/Menu/MenuText.h"
#include "Scripts/Menu/MenuLogic.h"

ReflectRegister(MenuScene);

using namespace Next;

void
MenuScene::OnSceneCreate()
{
	Entity entity = Entity::Create("Menu");
	entity.AddComponent<MenuLogic>();
	entity.AddComponent<MenuText>();

	PlayerPersistentData::ResetData();
}
