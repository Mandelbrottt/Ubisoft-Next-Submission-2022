#pragma once

#include "Scene.h"

#include "Reflection/Type.h"

namespace Next::SceneManager
{
	extern
	Reflection::Type const&
	CurrentSceneType();

	extern
	std::string const&
	CurrentSceneName();
	
	extern
	std::string const&
	CurrentSceneFullName();
	
	template<typename TScene>
	static
	bool
	IsCurrentScene()
	{
		static_assert(std::is_convertible_v<TScene*, Scene*>);
		return CurrentSceneType().GetTypeId() == Reflection::GetTypeId<TScene>();
	}

	extern
	bool
	IsCurrentScene(Reflection::Type const& a_type);

	template<typename TScene>
	static
	bool
	LoadScene()
	{
		return SceneManager::LoadScene(Reflection::Type::Get<TScene>());
	}

	extern
	bool
	LoadScene(Reflection::Type const& a_type);

	extern
	bool
	LoadScene(std::string_view const& a_typeName);
}
