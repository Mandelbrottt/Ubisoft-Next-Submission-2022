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
		mesh_container_t*    a_meshContainer
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
		if ((result = TryLoadModelFile<Detail::WavefrontModelLoader>(a_filename, extension, ".obj", &m_meshes))) {}

		return result;
	}
}
