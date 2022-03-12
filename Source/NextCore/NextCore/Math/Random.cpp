#include "pch.h"
#include "Random.h"

namespace Next::Random
{
	float
	Value()
	{
		const int cutoff = RAND_MAX;
		
		int value = rand() % cutoff;
		return static_cast<float>(value) / cutoff;
	}
}
