#include "pch.h"

#include "AudioSource.h"

namespace Next
{
	static _REFLECT_NAMESPACE TypeId _REFLECT_REGISTER_AudioSource = []()
	{
		_REFLECT_NAMESPACE Type::Register<AudioSource>();
		return _REFLECT_NAMESPACE GetTypeId<AudioSource>();
	}();
	
	bool
	AudioSource::Play(bool a_loop)
	{
		return audioClip.Play(a_loop);
	}

	bool
	AudioSource::Load(const char* a_fileName, bool a_playOnLoad, bool a_loop)
	{
		return audioClip.Load(a_fileName, a_playOnLoad, a_loop);
	}

	bool
	AudioSource::Load(std::string_view a_fileName, bool a_playOnLoad, bool a_loop)
	{
		return Load(a_fileName.data(), a_playOnLoad, a_loop);
	}

	bool
	AudioSource::Stop()
	{
		return audioClip.Stop();
	}

	bool
	AudioSource::IsPlaying() const
	{
		return audioClip.IsPlaying();
	}
}
