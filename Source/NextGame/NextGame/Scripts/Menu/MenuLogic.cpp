#include "MenuLogic.h"

#include "Scenes/LevelSelectScene.h"

ReflectRegister(MenuLogic);

using namespace Next;

void
MenuLogic::OnUpdate()
{
	if (Input::GetButtonDown(GamepadButton::A))
	{
		SceneManager::LoadScene<LevelSelectScene>();
	}
}
