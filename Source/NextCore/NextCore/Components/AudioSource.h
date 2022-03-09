#pragma once

#include "Reflection/Reflection.h"

#include "Scripting/Component.h"

#include "Audio/AudioClip.h"

#include "Math/Vector3.h"

namespace Next
{
	/**
	 * \brief Component representing a source of audio in the game world.
	 */
	class AudioSource : public Component
	{
		ReflectDeclare(AudioSource, Component)

	public:
		AudioClip* audioClip = nullptr;
		
		/**
		 * \brief Play the audio clip.
		 * \param a_loop Whether to loop the sound once it ends.
		 * \return True if the file could be played, false otherwise.
		 */
		bool
		Play(bool a_loop = false);
		
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
		[[nodiscard]]
		bool
		IsPlaying() const;
		
		ReflectMembers(
			ReflectField(audioClip, r_name = "AudioClip")
		)
	};
}
