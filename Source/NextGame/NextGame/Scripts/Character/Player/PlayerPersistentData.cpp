#include "PlayerPersistentData.h"

float PlayerPersistentData::fuel   = -1;
float PlayerPersistentData::health = -1;
float PlayerPersistentData::score  = -1;

void
PlayerPersistentData::ResetData()
{
	fuel   = 100;
	health = 5;
	score  = 0;
}
