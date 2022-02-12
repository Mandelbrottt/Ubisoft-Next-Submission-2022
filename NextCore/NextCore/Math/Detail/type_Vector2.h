#pragma once

#include "type_Vector.h"

#include "common_Vector.h"

#pragma warning( push )
#pragma warning( disable : 26495 ) // Possibly uninitialized member

#define VECTOR_SIZE 2

namespace NextCore::Math
{
	template<typename TUnderlying>
	struct Vector<VECTOR_SIZE, TUnderlying>
	{
		using value_type = TUnderlying;
		using type = Vector<VECTOR_SIZE, TUnderlying>;

		constexpr static TUnderlying size = VECTOR_SIZE;

		constexpr
		explicit
		Vector(TUnderlying a_value = 0)
			: x(a_value),
			  y(a_value) { }

		constexpr
		Vector(TUnderlying a_x, TUnderlying a_y)
			: x(a_x),
			  y(a_y) { }

		_VECTOR_GENERATE_CONSTRUCTORS();
		
	#pragma warning( push )
	#pragma warning( push )
	#pragma warning( disable : 4615 ) // Unknown user type
		union
		{
			struct
			{
				TUnderlying x;
				TUnderlying y;
			};

			struct
			{
				TUnderlying u;
				TUnderlying v;
			};

			TUnderlying data[VECTOR_SIZE];
		};
	#pragma warning( pop )
		
		_VECTOR_GENERATE_MEMBER_FUNCTIONS();
	};
}

#pragma warning( pop )

#undef VECTOR_SIZE
