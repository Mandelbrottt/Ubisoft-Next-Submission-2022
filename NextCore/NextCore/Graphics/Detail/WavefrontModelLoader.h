#pragma once

#include "ModelLoader.h"

namespace NextCore::Graphics::Detail
{
	class WavefrontModelLoader : public ModelLoader
	{
	public:
		bool
		LoadFromFile(std::string_view a_filename, file_stream_t& a_fileStream) override;
	};
}