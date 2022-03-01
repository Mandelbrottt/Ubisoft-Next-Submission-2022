#pragma once

#include "Graphics/CubeMap.h"

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
		ReflectDeclare(Camera, Component)

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
		
		CubeMap const&
		GetSkybox() const
		{
			return m_skybox;
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

		void
		SetSkybox(CubeMap const& a_skybox)
		{
			m_skybox = a_skybox;
		}

	private:
		float m_verticalFov = 60;
		
		float m_nearClippingPlane = 0.1f;
		float m_farClippingPlane  = 1000.f;

		CubeMap m_skybox;

		ReflectMembers(
			ReflectField(m_verticalFov)
			ReflectField(m_nearClippingPlane)
			ReflectField(m_farClippingPlane)
			ReflectField(m_skybox)
		)
	};
}
