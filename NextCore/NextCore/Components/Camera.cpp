#include "pch.h"
#include "Camera.h"

#include "Application/Application.h"

namespace Next
{
	ReflectRegister(Camera);
	
	float
	Camera::GetFov(FovType a_type) const
	{
		if (a_type == FovType::Vertical)
		{
			return m_verticalFov;
		}

		// Convert outgoing horizontal fov to vertical
		float x_on_y_aspect = Application::ScreenWidth() / Application::ScreenHeight();

		return m_verticalFov * x_on_y_aspect;
	}

	void
	Camera::SetFov(float a_fov, FovType a_type)
	{
		if (a_type == FovType::Vertical)
		{
			m_verticalFov = a_fov;
			return;
		}

		// Convert incoming vertical fov to horizontal
		float y_on_x_aspect = Application::ScreenHeight() / Application::ScreenWidth();

		m_verticalFov = a_fov * y_on_x_aspect;
	}
}
