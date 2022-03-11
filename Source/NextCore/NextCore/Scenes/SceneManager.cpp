#include "pch.h"
#include "SceneManager.h"

#include "Scene.h"

extern Next::Scene* g_mainLoop_sceneToChangeTo;

namespace Next::SceneManager
{
	struct SceneInfo
	{
		Reflection::TypeId typeId;

		Scene* scene;
	};

	static std::vector<SceneInfo> g_sceneInfos;

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

		if (g_mainLoop_sceneToChangeTo == nullptr)
		{
			g_mainLoop_sceneToChangeTo = scene;
		}
	}

	extern
	void
	RegisterAllScenes()
	{
		auto& types = Reflection::Type::Enumerate();
		
		for (auto const& [typeId, type] : types)
		{
			puts(type.FullName().c_str());
		
			auto const& sceneType = Reflection::Type::Get<Scene>();

			if (type == sceneType || !type.IsConvertibleTo(sceneType))
			{
				continue;
			}

			RegisterScene(type);
		}
	}

	bool
	ChangeScene(Reflection::Type const& a_type)
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

		if (iter == g_sceneInfos.end())
		{
			return false;
		}

		g_mainLoop_sceneToChangeTo = iter->scene;
		
		return true;
	}

	bool
	ChangeScene(std::string_view const& a_typeName)
	{
		Reflection::Type* type = Reflection::Type::TryGet(a_typeName);

		if (type == nullptr)
		{
			return false;
		}

		return ChangeScene(*type);
	}
}
