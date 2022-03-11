#pragma once

namespace Next::Math
{
	template<typename TScalar>
	constexpr
	TScalar
	Sign(TScalar a_value)
	{
		if (a_value < 0)
		{
			return -1;
		}

		if (a_value > 0)
		{
			return 1;
		}
		
		return 0;
	}
}