#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector3.h"

#include "Scripting/Component.h"

namespace Next
{
	class Transform : public Component
	{
		GenerateConstructors(Transform)

	public:
		Transform*
		GetParent();

		Transform const*
		GetParent() const;

		EntityId
		GetParentId() const;

		void
		SetParent(Transform* a_parent);
		
		void
		SetParent(EntityId a_parent);

		// TODO: Use proxy class with overloaded assignment to set dirty flag
		Vector3&
		Position()
		{
			return m_position;
		}
		
		Vector3 const&
		Position() const
		{
			return m_position;
		}
		
		Vector3&
		Rotation()
		{
			return m_rotation;
		}
		
		Vector3 const&
		Rotation() const
		{
			return m_rotation;
		}
		
		Vector3&
		Scale()
		{
			return m_scale;
		}

		Vector3 const&
		Scale() const
		{
			return m_scale;
		}

		Matrix4
		GetTransformationMatrix() const;

	private:
		Vector3 m_position = Vector3(0);
		Vector3 m_rotation = Vector3(0); // TODO: Convert to quaternion once implemented
		Vector3 m_scale    = Vector3(1);

		EntityId m_parent;

		mutable Matrix4 m_cachedTransformationMatrix;

		mutable bool m_isMatrixDirty = true;
		
		REFLECT_DECLARE(Transform, Component)

		REFLECT_MEMBERS(
			REFLECT_FIELD(m_position)
			REFLECT_FIELD(m_rotation)
			REFLECT_FIELD(m_scale)
		)
	};
}
