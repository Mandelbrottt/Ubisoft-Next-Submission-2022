#include "pch.h"

namespace Math
{
	using namespace Next;
	
	TEST(Vector3, Addition)
	{
		{
			Vector3 lhs = { 1, 2, 3 };
			Vector3 rhs = { 4, 5, 6 };
	
			Vector3 result = lhs + rhs;
		
			EXPECT_EQ(result, Vector3(5, 7, 9));
		}
		{
			Vector3 lhs = {  1, -2,  3 };
			Vector3 rhs = { -4,  5, -6 };
	
			Vector3 result = lhs + rhs;
		
			EXPECT_EQ(result, Vector3(-3, 3, -3));
		}
	}
}