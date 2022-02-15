#include "pch.h"

#include "Time.h"

// Scaled time variables
static float g_deltaTime   = 0;
static float g_elapsedTime = 0;

// Unscaled time variables
static float g_unscaledDeltaTime   = 0;
static float g_unscaledElapsedTime = 0;

static float g_timeScale = 1;

namespace NextCore::Time
{
	float
	ElapsedTime()
	{
		return g_elapsedTime;
	}

	float
	DeltaTime()
	{
		return g_deltaTime;
	}

	float
	UnscaledElapsedTime()
	{
		return g_unscaledElapsedTime;
	}

	float
	UnscaledDeltaTime()
	{
		return g_unscaledDeltaTime;
	}

	float
	TimeScale()
	{
		return g_timeScale;
	}

	void
	SetTimeScale(float a_timeScale)
	{
		g_timeScale = std::max(0.0f, a_timeScale);
	}
	
	// Consumers need to declare Update manually so as to not expose it unnecessarily
	NEXT_CORE_EXPORT
	void
	Update(float a_deltaTime)
	{
		float scaledDeltaTime = a_deltaTime * g_timeScale;
		g_deltaTime           = scaledDeltaTime;

		g_elapsedTime += g_deltaTime;

		g_unscaledDeltaTime = a_deltaTime;
		g_unscaledElapsedTime += a_deltaTime;
	}
}
