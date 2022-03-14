#pragma once
#include "Reflection/Reflection.h"

extern
Next::Reflection::TypeId
_STARTING_SCENE;

#define StartingScene(_scene_) \
	_REFLECT_NAMESPACE TypeId\
	_STARTING_SCENE = _REFLECT_NAMESPACE Type::Get<_scene_>().GetTypeId()