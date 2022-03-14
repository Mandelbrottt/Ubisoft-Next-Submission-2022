#include "ControlsUi.h"

ReflectRegister(ControlsUi);

using namespace Next;

void
ControlsUi::OnUpdate()
{
	const float h_position = 0.45;

	float v_position = 1.0f;

	auto get_v_position = [&]
	{
		return v_position -= 0.1f;
	};

	Gui::Print("Thrust Forward / Backward: L-Stick", { h_position, get_v_position() });
	Gui::Print("Thrust Left / Right: R-Stick", { h_position, get_v_position() });
	Gui::Print("Thrust Up / Down: R-Trigger / L-Trigger", { h_position, get_v_position() });
	Gui::Print("Roll: Hold L-Bumper", { h_position, get_v_position() });
	Gui::Print("Shoot Main Cannon: R-Bumper", { h_position, get_v_position() });
	Gui::Print("Tractor Beam: A", { h_position, get_v_position() });
}
