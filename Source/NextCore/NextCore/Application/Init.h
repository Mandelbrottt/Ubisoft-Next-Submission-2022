#pragma once
#include "Reflection/Reflection.h"

/**
 * \brief User implemented method that is called on program-start after all engine initialization is complete
 */
//extern
//void
//Application_Init();

extern
Next::Reflection::TypeId
_STARTING_SCENE;

#define StartingScene(_scene_) \
	_REFLECT_NAMESPACE TypeId\
	_STARTING_SCENE = _REFLECT_NAMESPACE Type::Get<_scene_>().GetTypeId()