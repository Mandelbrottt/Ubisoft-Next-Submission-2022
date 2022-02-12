#pragma once

#include "Matrix.h"
#include "Vector.h"

#include "NextCore/Common.h"

namespace NextCore::Math
{
	NEXT_CORE_EXPORT
	extern
	Matrix4x4
	Perspective(float a_fieldOfView, float a_aspectRatio, float a_nearPlane, float a_farPlane);
}
