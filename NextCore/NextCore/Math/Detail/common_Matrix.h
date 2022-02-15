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
		Matrix<SizeRhsX, SizeShared, TUnderlying> result { 0 };
		
		// ReSharper disable CppBadChildStatementIndent
		for (int y = 0; y < SizeLhsY;   y++)
		for (int x = 0; x < SizeRhsX;   x++)
		for (int z = 0; z < SizeShared; z++)
		{
			TUnderlying left  = a_lhs.data[y * SizeShared + z];
			TUnderlying right = a_rhs.data[z * SizeRhsX   + x];
			TUnderlying product = left * right;
			result.data[y * SizeRhsX + x] += product;
		}
		// ReSharper restore CppBadChildStatementIndent

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

	template<typename TMatrix>
	constexpr
	TMatrix
	Identity(typename TMatrix::value_type a_diagonal = 1)
	{
		constexpr int size_x = TMatrix::size_x;
		constexpr int size_y = TMatrix::size_y;
		
		constexpr int min_size = size_x < size_y ? size_x : size_y;
			
		TMatrix result { 0 };
		for (int i = 0; i < min_size; i++) 
		{ 
			result.data[i * size_x + i] = a_diagonal; 
		} 
		return result; 
	}
	
	template<int SizeX, int SizeY = SizeX, typename TUnderlying = float>
	constexpr
	Matrix<SizeX, SizeY, TUnderlying>
	Identity(TUnderlying a_diagonal = 1)
	{
		using TMatrix = Matrix<SizeX, SizeY, TUnderlying>;
		return Identity<TMatrix>(a_diagonal);
	}

	template<int SizeX, int SizeY, typename TUnderlying>
	constexpr
	Matrix<SizeY, SizeX, TUnderlying>
	Transpose(Matrix<SizeX, SizeY, TUnderlying> const& a_value)
	{
		//constexpr int min_size = SizeX < SizeY ? SizeX : SizeY;

		Matrix<SizeY, SizeX, TUnderlying> result;
		
		// ReSharper disable CppBadChildStatementIndent
		for (int y = 0; y < SizeY; y++)
		for (int x = 0; x < SizeX; x++)
		{
			//result[x][y] = a_value[y][x];
			result.data[x * SizeY + y] = a_value.data[y * SizeX + x];
		}
		// ReSharper restore CppBadChildStatementIndent

		return result;
	}
	
	template<int SizeX, int SizeY, typename TUnderlying>
	constexpr
	Matrix<SizeX, SizeY, TUnderlying>
	Inverse(Matrix<SizeX, SizeY, TUnderlying> const& a_value)
	{
		// TODO: Implement Inverse and Determinant
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
		Vector<size_x, TUnderlying>& \
		operator [](int a_index)\
		{ \
			if (a_index >= size_y) \
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
			return ::NextCore::Math::Identity<type>(a_diagonal);\
		} \
		\
		constexpr \
		type \
		Transpose() \
		{ \
			return ::NextCore::Math::Transpose(*this);\
		} \
		static_assert(true) // Require caller to end line with a semicolon
}
