#pragma once

#pragma once

#include "common_Matrix.h"
#include "type_Matrix.h"
#include "type_Vector2.h"

#pragma warning( push )
#pragma warning( disable : 26495 ) // Possibly uninitialized member

#define MATRIX_SIZE 2

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
			: data { 0 } {}

		explicit
		Matrix(TUnderlying a_value)
			: data { a_value } { }

		Matrix(
			Vector<MATRIX_SIZE, TUnderlying> a_x,
			Vector<MATRIX_SIZE, TUnderlying> a_y
		) : cols { a_x, a_y } {}

	#pragma warning( push )
	#pragma warning( disable : 4615 ) // Unknown user type
		union
		{
			struct
			{
				TUnderlying m00, m01;
				TUnderlying m10, m11;
			};

			Vector<MATRIX_SIZE, TUnderlying> cols[MATRIX_SIZE];

			TUnderlying data[MATRIX_SIZE * MATRIX_SIZE];
		};
	#pragma warning( pop )

		_MATRIX_GENERATE_MEMBER_FUNCTIONS();
	};
}

#pragma warning( pop )

#undef MATRIX_SIZE
