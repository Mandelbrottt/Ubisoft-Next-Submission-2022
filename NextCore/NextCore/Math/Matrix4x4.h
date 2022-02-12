#pragma once

#include "Detail/type_Matrix4x4.h"

namespace NextCore::Math
{
	typedef Matrix<4, 4, float> Matrix4x4;
	static_assert(sizeof(Matrix4x4) == sizeof(Matrix4x4::data));
}