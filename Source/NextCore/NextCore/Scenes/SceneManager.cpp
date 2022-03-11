#include "pch.h"
#include "SceneManager.h"

#include "Scene.h"

// Readonly
extern Next::Scene* g_mainLoop_activeScene;

extern Next::Scene* g_mainLoop_sceneToChangeTo;

namespace Next::SceneManager
{
	struct SceneInfo
	{
		Reflection::TypeId typeId;

		Scene* scene;
	};

	static std::vector<SceneInfo> g_sceneInfos;
	
	Reflection::Type const&
	CurrentSceneType()
	{
		return g_mainLoop_activeScene->GetType();
	}

	std::string const&
	CurrentSceneName()
	{
		return CurrentSceneType().Name();
	}

	std::string const&
	CurrentSceneFullName()
	{
		return CurrentSceneType().FullName();
	}

	bool
	IsCurrentScene(Reflection::Type const& a_type)
	{
		return CurrentSceneType().GetTypeId() == a_type.GetTypeId();
	}

	void
	RegisterScene(Reflection::Type const& a_type)
	{
		if (!a_type.IsConvertibleTo(Reflection::Type::Get<Scene>()))
		{
			return;
		}
		
		auto typeId = a_type.GetTypeId();

		auto iter = std::find_if(
			g_sceneInfos.begin(),
			g_sceneInfos.end(),
			[&](SceneInfo const& a_info)
			{
				return typeId == a_info.typeId;
			}
		);

		if (iter != g_sceneInfos.end())
		{
			return;
		}

		auto scene = static_cast<Scene*>(a_type.GetFactory()->Construct());

		g_sceneInfos.push_back(SceneInfo { typeId, scene });
	}

	extern
	void
	RegisterAllScenes()
	{
		auto& types = Reflection::Type::Enumerate();
		
		for (auto const& [typeId, type] : types)
		{
			auto const& sceneType = Reflection::Type::Get<Scene>();

			if (type == sceneType || !type.IsConvertibleTo(sceneType))
			{
				continue;
			}

			RegisterScene(type);
		}
	}

	bool
	LoadScene(Reflection::Type const& a_type)
	{
		if (!a_type.IsConvertibleTo(Reflection::Type::Get<Scene>()))
		{
			return false;
		}
		
		auto typeId = a_type.GetTypeId();

		auto iter = std::find_if(
			g_sceneInfos.begin(),
			g_sceneInfos.end(),
			[&](SceneInfo const& a_info)
			{
				return typeId == a_info.typeId;
			}
		);

		Scene* scene;

		if (iter == g_sceneInfos.end())
		{
			scene = static_cast<Scene*>(a_type.GetFactory()->Construct());

			g_sceneInfos.push_back(SceneInfo { typeId, scene });
		} else
		{
			scene = iter->scene;
		}

		g_mainLoop_sceneToChangeTo = scene;
		
		return true;
	}

	bool
	LoadScene(std::string_view const& a_typeName)
	{
		Reflection::Type* type = Reflection::Type::TryGet(a_typeName);

		if (type == nullptr)
		{
			return false;
		}

		return LoadScene(*type);
	}
}
