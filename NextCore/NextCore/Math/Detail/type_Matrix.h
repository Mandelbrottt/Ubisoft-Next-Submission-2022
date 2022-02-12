#pragma once

#include "type_Vector.h"

namespace NextCore::Math
{
	/**
	 * \brief Row-Major arbitrary-dimensional representation of a matrix
	 * \tparam SizeX The number of columns in the matrix
	 * \tparam SizeY The number of rows in the matrix
	 * \tparam TUnderlying The underlying data type
	 */
	template<int SizeX, int SizeY, typename TUnderlying>
	struct Matrix
	{
		using value_type = TUnderlying;
		using type = Matrix<SizeX, SizeY, TUnderlying>;

		constexpr static Vector<2, int> size { SizeX, SizeY };
		
		TUnderlying data[SizeX * SizeY];
	};
}