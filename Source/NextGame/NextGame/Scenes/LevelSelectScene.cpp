#include "LevelSelectScene.h"

#include "Scripts/Player/PlayerShip.h"

ReflectRegister(LevelSelectScene);

using namespace Next;

void
LevelSelectScene::OnSceneCreate()
{
	Entity playerEntity = Entity::Create("Player");
	playerEntity.AddComponent<PlayerShip>();

	Entity cameraEntity = Entity::Create("Camera");
	auto camera = cameraEntity.AddComponent<Camera>();
	camera->SetFov(100, FovType::Horizontal);
	cameraEntity.Transform()->SetPosition({ 0, 30, 0 });
	cameraEntity.Transform()->SetRotation({ -90, 0, 0 });
}
