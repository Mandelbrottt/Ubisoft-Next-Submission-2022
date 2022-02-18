#pragma once

#include "type_Vector.h"

#include "common_Vector.h"

#pragma warning( push )
#pragma warning( disable : 26495 ) // Possibly uninitialized member

#define VECTOR_SIZE 3

namespace NextCore::Math
{
	template<typename TUnderlying>
	constexpr
	Vector<3, TUnderlying> Cross(Vector<3, TUnderlying>, Vector<3, TUnderlying>);

	template<typename TUnderlying>
	struct Vector<VECTOR_SIZE, TUnderlying>
	{
		using value_type = TUnderlying;
		using type = Vector<VECTOR_SIZE, TUnderlying>;
		
		constexpr static TUnderlying size = VECTOR_SIZE;

		constexpr
		Vector()
			: x(0),
			  y(0),
			  z(0) { }
		
		constexpr
		explicit
		Vector(TUnderlying a_value)
			: x(a_value),
			  y(a_value),
			  z(a_value) { }

		constexpr
		Vector(TUnderlying a_x, TUnderlying a_y)
			: x(a_x),
			  y(a_y),
			  z(0) { }
		
		constexpr
		Vector(Vector<2, TUnderlying> a_other)
			: x(a_other.data[0]),
			  y(a_other.data[1]),
			  z(0) { }

		constexpr
		Vector(TUnderlying a_x, TUnderlying a_y, TUnderlying a_z)
			: x(a_x),
			  y(a_y),
			  z(a_z) { }
		
		_VECTOR_GENERATE_CONSTRUCTORS();

	#pragma warning( push )
	#pragma warning( disable : 4615 ) // Unknown user type
		union
		{
			struct
			{
				TUnderlying x;
				TUnderlying y;
				TUnderlying z;
			};

			struct
			{
				TUnderlying r;
				TUnderlying g;
				TUnderlying b;
			};

			TUnderlying data[VECTOR_SIZE];
		};
	#pragma warning( pop )
		
		_VECTOR_GENERATE_MEMBER_FUNCTIONS();

		constexpr
		Vector<3, TUnderlying>
		Cross(Vector<3, TUnderlying> a_other)
		{
			return ::NextCore::Math::Cross(*this, a_other);
		}
	};

	template<typename TUnderlying>
	constexpr
	Vector<3, TUnderlying>
	Cross(Vector<3, TUnderlying> a_lhs, Vector<3, TUnderlying> a_rhs)
	{
		TUnderlying x_component = a_lhs.y * a_rhs.z - a_lhs.z * a_rhs.y;
		TUnderlying y_component = a_lhs.z * a_rhs.x - a_lhs.x * a_rhs.z;
		TUnderlying z_component = a_lhs.x * a_rhs.y - a_lhs.y * a_rhs.x;

		return Vector<3, TUnderlying>(x_component, y_component, z_component);
	}
}

#pragma warning( pop )

#undef VECTOR_SIZE
