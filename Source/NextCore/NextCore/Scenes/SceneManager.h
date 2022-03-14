#pragma once

#include "Scene.h"

#include "Reflection/Type.h"

namespace Next::SceneManager
{
	extern
	Reflection::Type const&
	CurrentSceneType();

	/**
	 * \return The raw type name of the current scene.
	 */
	extern
	std::string const&
	CurrentSceneName();
	
	/**
	 * \return The fully qualified name of the current scene.
	 */
	extern
	std::string const&
	CurrentSceneFullName();
	
	/**
	 * \tparam TScene The type of the scene to compare against
	 * \return If the current scene is of type TScene
	 */
	template<typename TScene>
	static
	bool
	IsCurrentScene()
	{
		static_assert(std::is_convertible_v<TScene*, Scene*>);
		return CurrentSceneType().GetTypeId() == Reflection::GetTypeId<TScene>();
	}

	/**
	 * \param a_type The type of the scene to compare against
	 * \return If the current scene is of type a_type
	 */
	extern
	bool
	IsCurrentScene(Reflection::Type const& a_type);

	/**
	 * \brief Loads the given scene at the end of the current frame, unloading the current scene.
	 * \tparam TScene The scene to load
	 * \return True on success, false on failure.
	 */
	template<typename TScene>
	static
	bool
	LoadScene()
	{
		return SceneManager::LoadScene(Reflection::Type::Get<TScene>());
	}
	
	/**
	 * \brief Loads the given scene at the end of the current frame, unloading the current scene.
	 * \param a_type The type of the scene to load
	 * \return True on success, false on failure.
	 */
	extern
	bool
	LoadScene(Reflection::Type const& a_type);
	
	/**
	 * \brief Loads the given scene at the end of the current frame, unloading the current scene.
	 * \param a_typeId The type id of the scene to load
	 * \return True on success, false on failure.
	 */
	extern
	bool
	LoadScene(Reflection::TypeId a_typeId);
	
	/**
	 * \brief Loads the given scene at the end of the current frame, unloading the current scene.
	 * \param a_typeName The fully qualified name of the scene to load
	 * \return True on success, false on failure.
	 */
	extern
	bool
	LoadScene(std::string_view const& a_typeName);
}
