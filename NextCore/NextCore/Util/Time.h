#pragma once

#include "NextCore/Common.h"

namespace NextCore::Time
{
	NEXT_CORE_EXPORT
	extern
	float
	ElapsedTime();

	NEXT_CORE_EXPORT
	extern
	float
	DeltaTime();
	
	NEXT_CORE_EXPORT
	extern
	float
	UnscaledElapsedTime();

	NEXT_CORE_EXPORT
	extern
	float
	UnscaledDeltaTime();

	NEXT_CORE_EXPORT
	extern
	float
	TimeScale();
	
	NEXT_CORE_EXPORT
	extern
	void
	SetTimeScale(float a_timeScale);
}