#pragma once

#include <Input/InputCodes.h>

constexpr Next::Axis HORIZONTAL_MOVE = Next::Axis::Horizontal;
constexpr Next::Axis FORWARD_MOVE   = Next::Axis::Vertical;

constexpr Next::Axis UPWARDS_MOVE   = Next::Axis::RightTrigger;
constexpr Next::Axis DOWNWARDS_MOVE = Next::Axis::LeftTrigger;

constexpr Next::Axis HORIZONTAL_LOOK = Next::Axis::HorizontalLook;
constexpr Next::Axis VERTICAL_LOOK   = Next::Axis::VerticalLook;

constexpr Next::GamepadButton SHOOT_PROJECTILE = Next::GamepadButton::RightBumper;
constexpr Next::GamepadButton TRACTOR_BEAM     = Next::GamepadButton::LeftBumper;
