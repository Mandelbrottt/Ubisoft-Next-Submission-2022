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
		if (!audioClip)
		{
			return false;
		}
		
		return audioClip->Play(a_loop);
	}
	
	bool
	AudioSource::Stop()
	{
		if (!audioClip)
		{
			return false;
		}
		
		return audioClip->Stop();
	}

	bool
	AudioSource::IsPlaying() const
	{
		if (!audioClip)
		{
			return false;
		}
		
		return audioClip->IsPlaying();
	}
}
