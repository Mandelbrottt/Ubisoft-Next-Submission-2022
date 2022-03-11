#pragma once

#include "Reflection/Type.h"

namespace Next::SceneManager
{
	//template<typename TScene>
	//static
	//void
	//RegisterScene()
	//{
	//	return SceneManager::RegisterScene(Reflection::Type::Get<TScene>());
	//}

	//extern
	//void
	//RegisterScene(Reflection::Type const& a_type);
	
	template<typename TScene>
	static
	bool
	ChangeScene()
	{
		return SceneManager::ChangeScene(Reflection::Type::Get<TScene>());
	}

	extern
	bool
	ChangeScene(Reflection::Type const& a_type);

	extern
	bool
	ChangeScene(std::string_view const& a_typeName);
}
