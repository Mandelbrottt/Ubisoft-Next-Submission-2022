#pragma once

#include "Detail/type_Matrix4.h"

namespace NextCore::Math
{
	typedef Matrix<4, 4, float> Matrix4;
	static_assert(sizeof(Matrix4) == sizeof(Matrix4::data));
}