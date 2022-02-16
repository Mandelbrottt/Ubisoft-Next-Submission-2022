#pragma once

#include "Reflection/Reflection.h"

#include "Scripting/Component.h"

#include "Audio/AudioClip.h"

#include "Math/Vector3.h"

namespace NextCore::Component
{
	/**
	 * \brief Component representing a source of audio in the game world.
	 */
	class AudioSource : public Scripting::Component
	{
		GenerateConstructors(AudioSource)

	public:
		Audio::AudioClip audioClip;
		
		/**
		 * \brief Play the audio clip.
		 * \param a_loop Whether to loop the sound once it ends.
		 * \return True if the file could be played, false otherwise.
		 */
		bool
		Play(bool a_loop = false);
		
		/**
		 * \brief Load an audio clip from the given file with the given parameters.
		 * \param a_fileName The name of the audio file to load.
		 * \param a_playOnLoad Whether to play the sound once it loads.
		 * \param a_loop Whether to loop the sound once it ends.
		 * \return True if the load succeeded, false otherwise.
		 */
		bool
		Load(const char* a_fileName, bool a_playOnLoad = false, bool a_loop = false);
		
		/**
		 * \brief Load an audio clip from the given file with the given parameters.
		 * \param a_fileName The name of the audio file to load.
		 * \param a_playOnLoad Whether to play the sound once it loads.
		 * \param a_loop Whether to loop the sound once it ends.
		 * \return True if the load succeeded, false otherwise.
		 */
		bool
		Load(std::string_view a_fileName, bool a_playOnLoad = false, bool a_loop = false);

		/**
		 * \brief Stop the current audio clip.
		 * \return True if the sound could be stopped, false otherwise
		 * \remark This function stops the sound, not pause it. Pausing is not supported.
		 */
		bool
		Stop();

		/**
		 * \return Whether the sound is currently playing.
		 */
		bool
		IsPlaying() const;
		
		REFLECT_DECLARE(AudioSource)

		REFLECT_MEMBERS(
			REFLECT_FIELD(audioClip, r_name = "Audio Clip")
		)
	};
}
