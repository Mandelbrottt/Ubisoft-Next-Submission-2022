#include "pch.h"
#include "Model.h"

#include <filesystem>

#include "ModelLoaderTypedefs.h"

#include "Detail/WavefrontModelLoader.h"

namespace std
{
	namespace fs = filesystem;
}

namespace Next
{
	template<typename TModelLoader>
	bool
	TryLoadModelFile(
		std::string_view     a_filename,
		std::fs::path const& a_extension,
		const char*          a_checkExtension,
		mesh_container_t&    a_meshContainer
	)
	{
		static_assert(std::is_convertible_v<TModelLoader*, Detail::IModelLoader*>);

		if (a_extension == a_checkExtension)
		{
			model_file_stream_t fileStream(a_filename);

			TModelLoader loader;
			
			return loader.LoadFromFile(a_filename, fileStream, a_meshContainer);
		}

		return false;
	}

	Model*
	Model::Create()
	{
		return new Model;
	}

	static std::unordered_map<std::string, Model*> g_models;
	
	Model*
	Model::Create(std::string_view a_filename)
	{
		std::fs::path path = a_filename;
		path = std::fs::canonical(path);
		std::string pathString = path.string();

		auto iter = g_models.find(pathString);
		if (iter != g_models.end())
		{
			return iter->second;
		}
		
		Model* result = Create();
		bool success = result->LoadFromFile(a_filename);
		if (success)
		{
			g_models[pathString] = result;
		}
		return result;
	}

	bool
	Model::LoadFromFile(std::string_view a_filename)
	{
		if (!std::fs::exists(a_filename))
		{
			return false;
		}

		std::fs::path path = a_filename;

		auto extension = path.extension();

		bool result;
		
		// TODO: Add more model file support
		if ((result = TryLoadModelFile<Detail::WavefrontModelLoader>(a_filename, extension, ".obj", m_meshes))) {}

		return result;
	}
}
