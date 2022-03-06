#include "pch.h"

namespace Math
{
	using namespace Next;

	TEST(Vector2, Addition)
	{
		{
			Vector2 lhs = { 1, 2 };
			Vector2 rhs = { 4, 5 };
	
			Vector2 result = lhs + rhs;
		
			EXPECT_EQ(result, Vector2(5, 7));
		}
		{
			Vector2 lhs = {  1, -2 };
			Vector2 rhs = { -4,  5 };
	
			Vector2 result = lhs + rhs;
		
			EXPECT_EQ(result, Vector2(-3, 3));
		}
	}
}