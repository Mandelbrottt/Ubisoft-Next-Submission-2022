#include "pch.h"

#include "AudioClip.h"

// Fix compiler error from including dsound.h in NextAPI/SimpleSound.h
#define near
#define far
#include <mmsystem.h>
#include <NextAPI/SimpleSound.h>
#undef near
#undef far

namespace Next
{
	AudioClip::AudioClip(std::string_view a_fileName, bool a_playOnLoad, bool a_loop)
	{
		if (a_fileName == nullptr)
		{
			return;
		}

		LoadFromFile(a_fileName, a_playOnLoad, a_loop);
	}
	
	AudioClip::~AudioClip()
	{
		Stop();
	}

	AudioClip*
	AudioClip::Create()
	{
		return new AudioClip;
	}

	AudioClip*
	AudioClip::Create(std::string_view a_fileName, bool a_playOnLoad, bool a_loop)
	{
		AudioClip* result = Create();
		result->LoadFromFile(a_fileName, a_playOnLoad, a_loop);
		return result;
	}

	bool
	AudioClip::Play(bool a_loop)
	{
		if (m_fileName.empty())
		{
			return false;
		}
		
		// See NextAPI/app.h App::PlaySound
		DWORD flags = (a_loop) ? DSBPLAY_LOOPING : 0;

		auto result = CSimpleSound::GetInstance().PlaySound(m_fileName.c_str(), flags);

		return result;
	}

	bool
	AudioClip::LoadFromFile(std::string_view a_fileName, bool a_playOnLoad, bool a_loop)
	{
		// See NextAPI/app.h App::PlaySound
		DWORD flags = (a_loop) ? DSBPLAY_LOOPING : 0;

	    //#define PLAY_TO_LOAD

		/* HACK: No way to load a sound from the API directly and check for success,
		         so play the sound and stop it in the same function is the best we can do */
		/* BUG: Sound is still playing even though we stop it later in the function,
		   just keep going and hope for the best */
	#ifdef PLAY_TO_LOAD
		if (!CSimpleSound::GetInstance().PlaySound(a_fileName, flags))
		{
			return false;
		}
		// Stop the current audio clip
		Stop();
	#endif

		m_fileName = a_fileName;

		if (a_playOnLoad)
		{
			bool result =
		#ifndef PLAY_TO_LOAD
				CSimpleSound::GetInstance().PlaySound(a_fileName.data(), flags);
				// Stop the new audio clip
				Stop();
		#else
				true;
		#endif

			return result;
		}

		return true;
	}

	bool
	AudioClip::Stop()
	{
		bool result = CSimpleSound::GetInstance().StopSound(m_fileName.c_str());

		return result;
	}

	bool
	AudioClip::IsPlaying() const
	{
		return CSimpleSound::GetInstance().IsPlaying(m_fileName.c_str());
	}
}
