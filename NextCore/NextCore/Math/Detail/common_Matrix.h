#pragma once

#include "type_Matrix.h"
//#include "type_Vector.h"

namespace NextCore::Math
{
	#define _MATRIX_DEFINE_BINARY_OPERATOR_SAME_TYPE_SIMPLE(_operator_) \
		template<int SizeX, int SizeY, typename TUnderlying>\
		constexpr\
		Matrix<SizeX, SizeY, TUnderlying>\
		operator _operator_(\
			Matrix<SizeX, SizeY, TUnderlying> const& a_lhs, \
			Matrix<SizeX, SizeY, TUnderlying> const& a_rhs  \
		)\
		{\
			Matrix<SizeX, SizeY, TUnderlying> result;\
			for (int i = 0; i < SizeX * SizeY; i++)\
			{\
				result.data[i] = a_lhs.data[i] _operator_ a_rhs.data[i];\
			}\
			return result;\
		}
	
	#define _MATRIX_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE_SIMPLE(_operator_) \
		constexpr\
		Matrix& \
		operator _operator_##=(Matrix a_other)\
		{ \
			*this = *this _operator_ a_other;\
			return *this; \
		}
	
	_MATRIX_DEFINE_BINARY_OPERATOR_SAME_TYPE_SIMPLE(+)
	
	_MATRIX_DEFINE_BINARY_OPERATOR_SAME_TYPE_SIMPLE(-)
	
	template<int SizeX, int SizeY, typename TUnderlying>
	constexpr
	Matrix<SizeX, SizeY, TUnderlying>
	operator *(Matrix<SizeX, SizeY, TUnderlying> a_lhs, TUnderlying a_rhs)
	{
		Matrix<SizeX, SizeY, TUnderlying> result;
		for (int i = 0; i < SizeX * SizeY; i++)
		{
			result.data[i] = a_lhs.data[i] * a_rhs;
		}
		return result;
	}
	
	template<int SizeShared, int SizeLhsY, int SizeRhsX, typename TUnderlying>
	constexpr
	Matrix<SizeRhsX, SizeShared, TUnderlying>
	operator *(Matrix<SizeShared, SizeLhsY, TUnderlying> a_lhs, Matrix<SizeRhsX, SizeShared, TUnderlying> a_rhs)
	{
		auto shared = SizeShared;
		auto lhsY   = SizeLhsY;
		auto rhsX   = SizeRhsX;
		
		Matrix<SizeRhsX, SizeShared, TUnderlying> result { 0 };
		
		for (int y = 0; y < lhsY; y++)
		for (int x = 0; x < rhsX; x++)
		for (int z = 0; z < shared; z++)
		{
			//TUnderlying left  = a_lhs.operator[](y).operator[](z);
			//TUnderlying right = a_rhs.operator[](z).operator[](x);
			//TUnderlying product = left * right;
			//result.operator[](y).operator[](x) += product;

			TUnderlying left  = a_lhs.data[y * shared + z];
			TUnderlying right = a_rhs.data[z * rhsX   + x];
			TUnderlying product = left * right;
			result.data[y * rhsX + x] += product;
		}

		return result;
	}
	
	template<int SizeShared, int SizeLhsY, typename TUnderlying>
	constexpr
	Vector<SizeShared, TUnderlying>
	operator *(Matrix<SizeShared, SizeLhsY, TUnderlying> a_lhs, Vector<SizeShared, TUnderlying> a_rhs)
	{
		std::size_t sizeof_data = sizeof(TUnderlying) * sizeof(SizeShared);
		
		Matrix<1, SizeShared, TUnderlying> rhsMatrix;
		std::memcpy(rhsMatrix.data, a_rhs.data, sizeof_data);

		Matrix<1, SizeShared, TUnderlying> product = a_lhs * rhsMatrix;
		
		Vector<SizeShared, TUnderlying> result;
		std::memcpy(result.data, product.data, sizeof_data);

		return result;
	}
		
	#define _MATRIX_GENERATE_MEMBER_FUNCTIONS() \
      /*_MATRIX_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(+) \
		_MATRIX_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(-) \
		_MATRIX_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(*) \
		_MATRIX_DEFINE_BINARY_OPERATOR_MEMBER_SAME_TYPE(/) \
		_MATRIX_DEFINE_BINARY_OPERATOR_MEMBER_UNDERLYING_TYPE(*) \
		_MATRIX_DEFINE_BINARY_OPERATOR_MEMBER_UNDERLYING_TYPE(/) \
	  */\
		constexpr \
		Vector<size.x, TUnderlying>& \
		operator [](int a_index)\
		{ \
			if (a_index >= size.y) \
			{ \
				throw "Index out of range!";\
			} \
			return cols[a_index];\
		} \
		\
		constexpr \
		static \
		type \
		Identity(TUnderlying a_diagonal = 1) \
		{ \
			int min = type::size.x < type::size.y ? type::size.x : type::size.y; \
			\
			type result { 0 };\
			for (int i = 0; i < min; i++) \
			{ \
				result.data[i] = a_diagonal; \
			} \
			return result; \
		} \
		static_assert(true) // Require caller to end line with a semicolon
}
