#pragma once

#include <string>

#include "Scripting/Object.h"

namespace Next
{
	/**
	 * \brief Represents an audio file that can be played and stopped. Does not support pausing.
	 * \remarks Currently only supports wav files
	 */
	class AudioClip : Object
	{
		AudioClip() = default;

		/**
		 * \brief Load an audio clip from the given file with the given parameters.
		 * \param a_fileName The name of the audio file to load.
		 * \param a_playOnLoad Whether to play the sound once it loads.
		 * \param a_loop Whether to loop the sound once it ends.
		 */
		explicit
		AudioClip(std::string_view a_fileName, bool a_playOnLoad = false, bool a_loop = false);

		~AudioClip() override;

	public:
		static
		AudioClip*
		Create();

		static
		AudioClip*
		Create(std::string_view a_fileName, bool a_playOnLoad = false, bool a_loop = false);

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
		LoadFromFile(std::string_view a_fileName, bool a_playOnLoad = false, bool a_loop = false);

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

		/**
		 * \return The fileName of the currently loaded audio clip. 
		 */
		std::string_view
		GetFileName() const
		{
			return m_fileName;
		}

	private:
		std::string m_fileName;
	};
}
