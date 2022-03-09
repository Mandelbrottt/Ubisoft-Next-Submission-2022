#include "pch.h"

#include "AudioSource.h"

namespace Next
{
	ReflectRegister(AudioSource);
	
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
