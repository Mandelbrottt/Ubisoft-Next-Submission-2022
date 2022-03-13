#include "DistanceLevelChangeLogic.h"

#include "Scripts/Character/Player/PlayerShip.h"

ReflectRegister(DistanceLevelChangeLogic);

using namespace Next;

void
DistanceLevelChangeLogic::OnFirstUpdate()
{
	m_playerTransform = PlayerShip::GetPlayerShipEntity().Transform();
}

bool
DistanceLevelChangeLogic::ShouldChangeLevel()
{
	auto position = m_playerTransform->GetPosition();

	return position.MagnitudeSquared() > Math::Square(distanceToLeave);
}
