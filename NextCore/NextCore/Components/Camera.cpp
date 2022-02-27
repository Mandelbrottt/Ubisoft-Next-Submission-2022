#include "pch.h"
#include "Camera.h"

#include "Application/Application.h"

namespace Next
{
	REFLECT_REGISTER(Camera);
	
	float
	Camera::GetFov(FovType a_type) const
	{
		if (a_type == FovType::Horizontal)
		{
			return m_horizontalFov;
		}

		// Convert outgoing horizontal fov to vertical
		float y_on_x_aspect = Application::ScreenHeight() / Application::ScreenWidth();

		return m_horizontalFov * y_on_x_aspect;
	}

	void
	Camera::SetFov(float a_fov, FovType a_type)
	{
		if (a_type == FovType::Horizontal)
		{
			m_horizontalFov = a_fov;
			return;
		}

		// Convert incoming vertical fov to horizontal
		float x_on_y_aspect = Application::ScreenWidth() / Application::ScreenHeight();

		m_horizontalFov = a_fov * x_on_y_aspect;
	}
}
