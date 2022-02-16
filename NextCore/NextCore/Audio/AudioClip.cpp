#include "pch.h"

#include "AudioClip.h"

// Fix compiler error from including dsound.h
#include <mmsystem.h>

#include <NextAPI/SimpleSound.h>

#include "NextAPI/app.h"

namespace NextCore::Audio
{
	AudioClip::AudioClip(const char* a_fileName, bool a_playOnLoad, bool a_loop)
	{
		if (a_fileName == nullptr)
		{
			return;
		}

		Load(a_fileName, a_playOnLoad, a_loop);
	}
	
	AudioClip::~AudioClip()
	{
		Stop();
	}

	bool
	AudioClip::Play(bool a_loop)
	{
		if (m_fileName.empty())
		{
			return false;
		}
		
		// See NextAPI/app.h
		DWORD flags = (a_loop) ? DSBPLAY_LOOPING : 0;

		auto result = CSimpleSound::GetInstance().PlaySound(m_fileName.c_str(), flags);

		return result;
	}

	bool
	AudioClip::Load(const char* a_fileName, bool a_playOnLoad, bool a_loop)
	{
		// See NextAPI/app.h
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
	#endif
		// Stop the current audio clip
		Stop();

		m_fileName = a_fileName;

		if (a_playOnLoad)
		{
			bool result =
		#ifndef PLAY_TO_LOAD
				CSimpleSound::GetInstance().PlaySound(a_fileName, flags);
		#else
				true;
		#endif

			return result;
		}

		// Stop the new audio clip
		Stop();

		return true;
	}

	bool
	AudioClip::Load(std::string_view a_fileName, bool a_playOnLoad, bool a_loop)
	{
		return Load(a_fileName.data(), a_playOnLoad, a_loop);
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
