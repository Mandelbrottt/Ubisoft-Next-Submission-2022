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
		GenerateConstructors(Camera)
	public:
		float
		GetFov(FovType a_type = FovType::Horizontal) const;

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
		SetFov(float a_fov, FovType a_type = FovType::Horizontal);

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
		float m_horizontalFov = 60;

		float m_nearClippingPlane = 0.1f;
		float m_farClippingPlane  = 1000.f;

		REFLECT_DECLARE(Camera, Component)

		REFLECT_MEMBERS(
			REFLECT_FIELD(m_horizontalFov)
			REFLECT_FIELD(m_nearClippingPlane)
			REFLECT_FIELD(m_farClippingPlane)
		)
	};
}
