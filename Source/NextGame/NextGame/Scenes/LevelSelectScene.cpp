#include "LevelSelectScene.h"

#include "FlatGroundScene.h"

ReflectRegister(LevelSelectScene);

using namespace Next;

void
LevelSelectScene::OnSceneCreate()
{
	SceneManager::LoadScene<FlatGroundScene>();
}
