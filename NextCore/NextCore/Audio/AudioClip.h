#pragma once

#include <string>

namespace NextCore::Audio
{
	class AudioClip
	{
	public:
		AudioClip() = default;

		explicit
		AudioClip(const char* a_fileName, bool a_playOnLoad = false, bool a_loop = false);

		explicit
		AudioClip(std::string_view a_fileName, bool a_playOnLoad = false, bool a_loop = false);

		~AudioClip();

		bool
		Play(bool a_loop = false);

		bool
		Load(const char* a_fileName, bool a_playOnLoad = false, bool a_loop = false);

		bool
		Load(std::string_view a_fileName, bool a_playOnLoad = false, bool a_loop = false);

		bool
		Stop();

		bool
		IsPlaying() const;

		std::string_view
		GetFileName() const
		{
			return m_fileName;
		}

	private:
		std::string m_fileName;
	};
}
