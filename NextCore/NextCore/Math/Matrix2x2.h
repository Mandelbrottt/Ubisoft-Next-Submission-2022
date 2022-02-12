#pragma once

#include "Detail/type_Matrix2x2.h"

namespace NextCore::Math
{
	typedef Matrix<2, 2, float> Matrix2x2;
	static_assert(sizeof(Matrix2x2) == sizeof(Matrix2x2::data));
}