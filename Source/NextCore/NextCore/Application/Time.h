#pragma once

#include "NextCoreCommon.h"

namespace Next::Time
{
	/**
	 * \return Time in seconds since the program was started.
	 * \remark This value is scaled by \link TimeScale \endlink. If you want the real time since the
	 *         program started, use \link UnscaledElapsedTime \endlink instead.
	 */
	NEXT_CORE_EXPORT
	extern
	float
	ElapsedTime();

	/**
	 * \return Time in seconds since the last frame.
	 * \remark This value is scaled by \link TimeScale \endlink. If you want the real time since the
	 *         program started, use \link UnscaledElapsedTime \endlink instead.
	 */
	NEXT_CORE_EXPORT
	extern
	float
	DeltaTime();

	/**
	 * \return Real Time in seconds since the program was started.
	 */
	NEXT_CORE_EXPORT
	extern
	float
	UnscaledElapsedTime();

	/**
	 * \return Time in seconds since the last frame.
	 */
	NEXT_CORE_EXPORT
	extern
	float
	UnscaledDeltaTime();

	/**
	 * \brief The value by which \link DeltaTime \endling is scaled by each frame.
	 * \return The current timescale.
	 */
	NEXT_CORE_EXPORT
	extern
	float
	TimeScale();
	
	/**
	 * \brief The value by which \link DeltaTime \endling is scaled by each frame.
	 * \param a_timeScale The timescale to set.
	 */
	NEXT_CORE_EXPORT
	extern
	void
	SetTimeScale(float a_timeScale);
}