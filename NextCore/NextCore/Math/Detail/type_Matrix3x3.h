#pragma once

#pragma once

#include "common_Matrix.h"
#include "type_Matrix.h"
#include "type_Vector3.h"

#pragma warning( push )
#pragma warning( disable : 26495 ) // Possibly uninitialized member

#define MATRIX_SIZE 3

namespace NextCore::Math
{
	template<typename TUnderlying>
	struct Matrix<MATRIX_SIZE, MATRIX_SIZE, TUnderlying>
	{
		using value_type = TUnderlying;
		using type = Matrix<MATRIX_SIZE, MATRIX_SIZE, TUnderlying>;

		constexpr static int size_x = MATRIX_SIZE;
		constexpr static int size_y = MATRIX_SIZE;

		Matrix()
			: Matrix(0) {}

		explicit
		Matrix(TUnderlying a_value)
			: data { a_value } { }

		Matrix(
			Vector<MATRIX_SIZE, TUnderlying> a_x,
			Vector<MATRIX_SIZE, TUnderlying> a_y,
			Vector<MATRIX_SIZE, TUnderlying> a_z
		) : cols { a_x, a_y, a_z } {}

	#pragma warning( push )
	#pragma warning( push )
	#pragma warning( disable : 4615 ) // Unknown user type
		union
		{
			struct
			{
				TUnderlying m00, m01, m02;
				TUnderlying m10, m11, m12;
				TUnderlying m20, m21, m22;
			};

			Vector<size_x, TUnderlying> cols[size_y];

			TUnderlying data[size_x * size_y];
		};
	#pragma warning( pop )

		_MATRIX_GENERATE_MEMBER_FUNCTIONS();
	};
}

#pragma warning( pop )

#undef MATRIX_SIZE
