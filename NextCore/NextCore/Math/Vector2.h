#pragma once

#include "Detail/type_Vector2.h"

namespace NextCore::Math
{
	typedef Vector<2, float> Vector2f;
	static_assert(sizeof(Vector2f) == sizeof(Vector2f::data));

	typedef Vector2f Vector2;
	
	typedef Vector<2, int> Vector2i;
	static_assert(sizeof(Vector2i) == sizeof(Vector2i::data));
	
	typedef Vector<2, unsigned int> Vector2u;
	static_assert(sizeof(Vector2u) == sizeof(Vector2u::data));
}