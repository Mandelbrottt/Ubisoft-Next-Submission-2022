#pragma once

#include "Detail/type_Matrix3x3.h"

namespace NextCore::Math
{
	typedef Matrix<3, 3, float> Matrix3x3;
	static_assert(sizeof(Matrix3x3) == sizeof(Matrix3x3::data));
}