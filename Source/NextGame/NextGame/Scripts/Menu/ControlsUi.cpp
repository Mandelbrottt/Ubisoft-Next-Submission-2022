#include "ControlsUi.h"

ReflectRegister(ControlsUi);

using namespace Next;

void
ControlsUi::OnUpdate()
{
	const float h_position = 0.45;

	Gui::Print("Thrust Forward / Backward: L-Stick", { h_position, 0.9 });
	Gui::Print("Thrust Left / Right: R-Stick", { h_position, 0.8 });
	Gui::Print("Thrust Up / Down: R-Trigger / L-Trigger", { h_position, 0.7 });
	Gui::Print("Shoot Main Cannon: R-Bumper", { h_position, 0.6 });
	Gui::Print("Tractor Beam: A", { h_position, 0.5 });
}
