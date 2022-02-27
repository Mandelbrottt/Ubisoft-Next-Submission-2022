#pragma once

#include "NextCoreCommon.h"
#include "type_Vector.h"

namespace Next
{
	/**
	 * \brief Row-Major arbitrary-dimensional representation of a matrix
	 * \tparam SizeX The number of columns in the matrix
	 * \tparam SizeY The number of rows in the matrix
	 * \tparam TUnderlying The underlying data type
	 * \remark The name uses _t type notation because for the API we want, there is no way to
	 *         have a template and non-template type share an identifier.
	 *         ie.
	 *         \code
	 *         struct Matrix
	 *         {
	 *		       ...
	 *		   }
	 *		   template<...>
	 *		   struct Matrix
	 *		   {
	 *		       ...
	 *		   } // Compile error, redefinition
	 *		   \endcode
	 *	   <br>All matrix specializations should implement the _MATRIX_GENERATE_MEMBER_FUNCTIONS() macro.
	 */
	template<int SizeX, int SizeY, typename TUnderlying>
	struct Matrix_t
	{
		using value_type = TUnderlying;
		using type = Matrix_t;

		constexpr static int size_x = SizeX;
		constexpr static int size_y = SizeY;

		TUnderlying data[SizeX * SizeY];
	};

	namespace Matrix
	{
		template<typename TMatrix>
		static
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
		static
		constexpr
		Matrix_t<SizeX, SizeY, TUnderlying>
		Identity(TUnderlying a_diagonal = 1)
		{
			using TMatrix = Matrix_t<SizeX, SizeY, TUnderlying>;
			return Identity<TMatrix>(a_diagonal);
		}

		template<int SizeX, int SizeY, typename TUnderlying>
		constexpr
		Matrix_t<SizeY, SizeX, TUnderlying>
		Transpose(Matrix_t<SizeX, SizeY, TUnderlying> const& a_value)
		{
			//constexpr int min_size = SizeX < SizeY ? SizeX : SizeY;

			Matrix_t<SizeY, SizeX, TUnderlying> result;

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
		Matrix_t<SizeX, SizeY, TUnderlying>
		Inverse(Matrix_t<SizeX, SizeY, TUnderlying> const& a_value)
		{
			// TODO: Implement Inverse and Determinant
		}
	}
	
	// Matrix-Matrix Addition
	template<int SizeX, int SizeY, typename TUnderlying>
	constexpr
	Matrix_t<SizeX, SizeY, TUnderlying>
	operator +(Matrix_t<SizeX, SizeY, TUnderlying> const& a_lhs, Matrix_t<SizeX, SizeY, TUnderlying> const& a_rhs)
	{
		Matrix_t<SizeX, SizeY, TUnderlying> result;
		for (int i = 0; i < SizeX * SizeY; i++)
		{
			result.data[i] = a_lhs.data[i] + a_rhs.data[i];
		}
		return result;
	}

	// Matrix-Matrix Subtraction
	template<int SizeX, int SizeY, typename TUnderlying>
	constexpr
	Matrix_t<SizeX, SizeY, TUnderlying>
	operator -(Matrix_t<SizeX, SizeY, TUnderlying> const& a_lhs, Matrix_t<SizeX, SizeY, TUnderlying> const& a_rhs)
	{
		Matrix_t<SizeX, SizeY, TUnderlying> result;
		for (int i = 0; i < SizeX * SizeY; i++)
		{
			result.data[i] = a_lhs.data[i] - a_rhs.data[i];
		}
		return result;
	}

	// Matrix-Scalar Multiplication
	template<int SizeX, int SizeY, typename TUnderlying>
	constexpr
	Matrix_t<SizeX, SizeY, TUnderlying>
	operator *(Matrix_t<SizeX, SizeY, TUnderlying> a_lhs, TUnderlying a_rhs)
	{
		Matrix_t<SizeX, SizeY, TUnderlying> result;
		for (int i = 0; i < SizeX * SizeY; i++)
		{
			result.data[i] = a_lhs.data[i] * a_rhs;
		}
		return result;
	}

	// Matrix-Matrix Multiplication
	template<int SizeShared, int SizeLhsY, int SizeRhsX, typename TUnderlying>
	constexpr
	Matrix_t<SizeRhsX, SizeShared, TUnderlying>
	operator *(Matrix_t<SizeShared, SizeLhsY, TUnderlying> a_lhs, Matrix_t<SizeRhsX, SizeShared, TUnderlying> a_rhs)
	{
		Matrix_t<SizeRhsX, SizeShared, TUnderlying> result { 0 };

		const auto sizeX      = SizeRhsX;
		const auto sizeY      = SizeLhsY;
		const auto sizeShared = SizeShared;

		// ReSharper disable CppBadChildStatementIndent
		for (int y = 0; y < SizeLhsY; y++)
			for (int x = 0; x < SizeRhsX; x++)
				for (int z = 0; z < SizeShared; z++)
				{
					auto        leftIndex = y * SizeShared + z;
					TUnderlying left      = a_lhs.data[leftIndex];

					auto        rightIndex = y * SizeRhsX + x;
					TUnderlying right      = a_rhs.data[rightIndex];

					TUnderlying product = left * right;
					result.data[z * SizeRhsX + x] += product;
				}
		// ReSharper restore CppBadChildStatementIndent

		return result;
	}

	// Matrix-Vector Multiplication Specialization
	template<int SizeShared, int SizeLhsY, typename TUnderlying>
	constexpr
	Vector_t<SizeShared, TUnderlying>
	operator *(Matrix_t<SizeShared, SizeLhsY, TUnderlying> a_lhs, Vector_t<SizeShared, TUnderlying> a_rhs)
	{
		constexpr int size_shared = SizeShared;
		constexpr int size_lhs_y  = SizeLhsY;

		// Don't use SizeShared directly, causes stack corruptions on some systems
		std::size_t sizeof_data = sizeof(TUnderlying) * size_shared;

		Matrix_t<1, SizeShared, TUnderlying> rhsMatrix;
		std::memcpy(rhsMatrix.data, a_rhs.data, sizeof_data);

		Matrix_t<1, SizeShared, TUnderlying> product = a_lhs * rhsMatrix;

		Vector_t<SizeShared, TUnderlying> result;
		std::memcpy(result.data, product.data, sizeof_data);

		return result;
	}

	#define _MATRIX_GENERATE_MEMBER_FUNCTIONS() \
		constexpr\
		Matrix_t& \
		operator *=(Matrix_t a_other)\
		{ \
			*this = *this * a_other;\
			return *this; \
		} \
		\
		constexpr \
		Vector_t<size_x, TUnderlying>& \
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
			return ::Next::Matrix::Identity<type>(a_diagonal);\
		} \
		\
		constexpr \
		type& \
		Transpose() \
		{ \
			*this = ::Next::Matrix::Transpose(*this);\
			return *this;\
		} \
		_MACRO_REQUIRE_SEMICOLON // Require caller to end line with a semicolon
}
