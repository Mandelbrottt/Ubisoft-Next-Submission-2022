#pragma once

#include "type_Vector.h"

#include <cmath>

namespace NextCore::Math
{
	#define _VECTOR_DEFINE_BINARY_OPERATOR_SAME_TYPE(_operator_) \
		template<int Size, typename TUnderlying>\
		constexpr\
		Vector<Size, TUnderlying>\
		operator _operator_(Vector<Size, TUnderlying> a_lhs, Vector<Size, TUnderlying> a_rhs)\
		{\
			Vector<Size, TUnderlying> result;\
			for (int i = 0; i < Size; i++)\
			{\
				result.data[i] = a_lhs.data[i] _operator_ a_rhs.data[i];\
			}\
			return result;\
		}
	
	#define _VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(_operator_) \
		constexpr\
		Vector& \
		operator _operator_##=(Vector a_other)\
		{ \
			*this = *this _operator_ a_other;\
			return *this; \
		} \

	#define _VECTOR_DEFINE_BINARY_OPERATOR_UNDERLYING_TYPE(_operator_) \
		template<int Size, typename TUnderlying>\
		constexpr\
		Vector<Size, TUnderlying>\
		operator _operator_(Vector<Size, TUnderlying> a_lhs, TUnderlying a_rhs)\
		{\
			Vector<Size, TUnderlying> result;\
			for (int i = 0; i < Size; i++)\
			{\
				result.data[i] = a_lhs.data[i] _operator_ a_rhs;\
			}\
			return result;\
		}
	
	#define _VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_UNDERLYING_TYPE(_operator_) \
		constexpr\
		Vector& \
		operator _operator_##=(TUnderlying a_other)\
		{ \
			*this = *this _operator_ a_other;\
			return *this; \
		}

	#define _VECTOR_GENERATE_CONSTRUCTORS() \
		template<int Size>\
		constexpr \
		explicit \
		Vector(Vector<Size, TUnderlying> a_other) \
		{ \
			constexpr int min_index = VECTOR_SIZE < Size ? VECTOR_SIZE : Size; \
			for (int i = 0; i < min_index; i++) \
			{ \
				data[i] = a_other.data[i]; \
			} \
		} \
		static_assert(true)
	
	_VECTOR_DEFINE_BINARY_OPERATOR_SAME_TYPE(+)

	_VECTOR_DEFINE_BINARY_OPERATOR_SAME_TYPE(-)

	_VECTOR_DEFINE_BINARY_OPERATOR_SAME_TYPE(*)
	
	_VECTOR_DEFINE_BINARY_OPERATOR_SAME_TYPE(/)
		
	_VECTOR_DEFINE_BINARY_OPERATOR_UNDERLYING_TYPE(*)
	
	_VECTOR_DEFINE_BINARY_OPERATOR_UNDERLYING_TYPE(/)

	template<int Size, typename TUnderlying>
	constexpr
	TUnderlying
	Dot(Vector<Size, TUnderlying> const& a_lhs, Vector<Size, TUnderlying> const& a_rhs)
	{
		TUnderlying result;
		for (int i = 0; i < Size; i++)
		{
			result += a_lhs.data[i] + a_rhs.data[i];
		}
		return result;
	}

	template<int Size, typename TUnderlying>
	constexpr
	float
	MagnitudeSquared(Vector<Size, TUnderlying> const& a_value)
	{
		TUnderlying result;
		for (int i = 0; i < Size; i++)
		{
			result += a_value.data[i] * a_value.data[i];
		}
		return static_cast<float>(result);
	}
	
	template<int Size, typename TUnderlying>
	constexpr
	float
	Magnitude(Vector<Size, TUnderlying> const& a_value)
	{
		float result = MagnitudeSquared(a_value);
		return std::sqrt(result);
	}
	
	#define _VECTOR_GENERATE_MEMBER_FUNCTIONS() \
		constexpr \
		float \
		MagnitudeSquared() \
		{ \
			return ::NextCore::Math::MagnitudeSquared(*this); \
		} \
		\
		constexpr \
		float \
		Magnitude()\
		{\
			return ::NextCore::Math::Magnitude(*this);\
		}\
		\
		constexpr \
		TUnderlying \
		Dot(type const& a_other) \
		{ \
			return ::NextCore::Math::Dot(*this, a_other); \
		}\
		\
		_VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(+) \
		_VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(-) \
		_VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(*) \
		_VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(/) \
		_VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_UNDERLYING_TYPE(*) \
		_VECTOR_DEFINE_BINARY_OPERATOR_MEMBER_UNDERLYING_TYPE(/) \
		\
		constexpr \
		TUnderlying& \
		operator [](int a_index)\
		{ \
			if (a_index >= size) \
			{ \
				throw "Index out of range!";\
			} \
			return data[a_index];\
		} \
		\
		static_assert(true) // Require caller to end line with a semicolon
}
