#pragma once

#pragma once

#include "type_Matrix.h"

#include "common_Matrix.h"

#pragma warning( push )
#pragma warning( disable : 26495 ) // Possibly uninitialized member

#define MATRIX_SIZE 3

namespace NextCore::Math
{
	template<typename TUnderlying>
	struct Matrix<MATRIX_SIZE, MATRIX_SIZE, TUnderlying>
	{
		using value_type = TUnderlying;
		using type = Vector<MATRIX_SIZE, TUnderlying>;

		constexpr static Vector<2, int> size = { MATRIX_SIZE, MATRIX_SIZE };

		explicit Matrix(TUnderlying a_value = 0)
			: cols {
				Vector<MATRIX_SIZE, TUnderlying>(a_value), 
				Vector<MATRIX_SIZE, TUnderlying>(a_value),
				Vector<MATRIX_SIZE, TUnderlying>(a_value)
			} { }

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

			struct
			{
				Vector<MATRIX_SIZE, TUnderlying> cols[MATRIX_SIZE];
			};

			TUnderlying data[MATRIX_SIZE * MATRIX_SIZE];
		};
	#pragma warning( pop )
		
		_MATRIX_GENERATE_MEMBER_FUNCTIONS();
	};
}

#pragma warning( pop )

#undef MATRIX_SIZE
