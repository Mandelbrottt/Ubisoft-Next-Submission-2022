#pragma once

#include "Detail/type_Vector3.h"

namespace NextCore::Math
{
	typedef Vector<3, float> Vector3f;
	static_assert(sizeof(Vector3f) == sizeof(Vector3f::data));

	typedef Vector3f Vector3;
	
	typedef Vector<3, int> Vector3i;
	static_assert(sizeof(Vector3i) == sizeof(Vector3i::data));
	
	typedef Vector<3, unsigned int> Vector3u;
	static_assert(sizeof(Vector3u) == sizeof(Vector3u::data));
}