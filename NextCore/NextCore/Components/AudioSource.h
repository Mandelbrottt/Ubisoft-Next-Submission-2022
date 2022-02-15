#pragma once

#include "Reflection/Reflection.h"

#include "Scripting/Component.h"

#include "Audio/AudioClip.h"

#include "Math/Vector3.h"

namespace NextCore::Component
{
	class AudioSource : public Scripting::Component
	{
		GenerateConstructors(AudioSource);

	public:
		Audio::AudioClip audioClip;
		
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
		
		REFLECT_DECLARE(AudioSource);

		REFLECT_MEMBERS(
			REFLECT_FIELD(audioClip, r_name = "Audio Clip")
		);
	};
}
