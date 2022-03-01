#pragma once

#include "Scripting/Component.h"

namespace Next
{
	enum class FovType
	{
		Horizontal,
		Vertical,
	};

	class Camera : public Component
	{
	public:
		float
		GetFov(FovType a_type = FovType::Vertical) const;

		float
		GetNearClippingPlane() const
		{
			return m_nearClippingPlane;
		}

		float
		GetFarClippingPlane() const
		{
			return m_farClippingPlane;
		}

		void
		SetFov(float a_fov, FovType a_type = FovType::Vertical);

		void
		SetNearClippingPlane(float a_near)
		{
			m_nearClippingPlane = a_near;
		}

		void
		SetFarClippingPlane(float a_far)
		{
			m_farClippingPlane = a_far;
		}

	private:
		float m_verticalFov = 60;
		
		float m_nearClippingPlane = 0.1f;
		float m_farClippingPlane  = 1000.f;

		ReflectDeclare(Camera, Component)

		ReflectMembers(
			ReflectField(m_verticalFov)
			ReflectField(m_nearClippingPlane)
			ReflectField(m_farClippingPlane)
		)
	};
}
