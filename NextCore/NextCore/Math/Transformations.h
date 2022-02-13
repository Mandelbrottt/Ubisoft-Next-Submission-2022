#pragma once

#include "Matrix4.h"

#include "Common.h"

namespace NextCore::Math
{
	NEXT_CORE_EXPORT
	extern
	Matrix4
	Perspective(float a_fieldOfView, float a_aspectRatio, float a_nearPlane, float a_farPlane);
}
