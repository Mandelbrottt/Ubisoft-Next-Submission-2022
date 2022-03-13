#pragma once

#include <MinimalInclude.h>

class PlayerPersistentData
{
public:
	static float fuel;
	static float health;
	static float score;

	static
	void
	ResetData();
};
