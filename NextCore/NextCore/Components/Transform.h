#pragma once

#include "Math/Vector3.h"

#include "Scripting/Component.h"

namespace NextCore::Component
{
	class Transform : public Scripting::Component
	{
		GenerateConstructors(Transform)

	public:
		Math::Vector3&
		Position()
		{
			return m_position;
		}
		
		Math::Vector3 const&
		Position() const
		{
			return m_position;
		}
		
		Math::Vector3&
		Rotation()
		{
			return m_rotation;
		}
		
		Math::Vector3 const&
		Rotation() const
		{
			return m_rotation;
		}
		
		Math::Vector3&
		Scale()
		{
			return m_scale;
		}

		Math::Vector3 const&
		Scale() const
		{
			return m_scale;
		}

	private:
		Math::Vector3 m_position = Math::Vector3(0);
		Math::Vector3 m_rotation = Math::Vector3(0); // TODO: Convert to quaternion once implemented
		Math::Vector3 m_scale    = Math::Vector3(1);
		
		REFLECT_DECLARE(Transform, Component)

		REFLECT_MEMBERS(
			REFLECT_FIELD(m_position)
			REFLECT_FIELD(m_rotation)
			REFLECT_FIELD(m_scale)
		)
	};
}
