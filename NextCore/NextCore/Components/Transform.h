#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector3.h"

#include "Scripting/Component.h"

namespace Next
{
	/* TODO: Find way to have single function to get and set properties.
	         VC++ properties would be great if it was portable */
	
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
		
		Vector3 const&
		GetPosition()
		{
			return m_position;
		}
		
		Vector3 const&
		SetPosition(Vector3 const a_position)
		{
			m_isMatrixDirty |= a_position != m_position;
			
			return m_position = a_position;
		}
		
		Vector3 const&
		GetPosition() const
		{
			return m_position;
		}
		
		Vector3 const&
		GetRotation()
		{
			return m_rotation;
		}
		
		Vector3 const&
		SetRotation(Vector3 const a_rotation)
		{
			m_isMatrixDirty |= a_rotation != m_rotation;
			
			return m_rotation = a_rotation;
		}
		
		Vector3 const&
		GetRotation() const
		{
			return m_rotation;
		}
		
		Vector3 const&
		GetScale()
		{
			return m_scale;
		}
		
		Vector3 const&
		SetScale(Vector3 const a_scale)
		{
			m_isMatrixDirty |= a_scale == m_scale;
			
			return m_scale = a_scale;
		}

		Vector3 const&
		GetScale() const
		{
			return m_scale;
		}

		Vector3 Right() const
		{
			auto transformationMatrix = GetTransformationMatrix();
			return Vector3(transformationMatrix[0]);
		}
		
		Vector3 Up() const
		{
			auto transformationMatrix = GetTransformationMatrix();
			return Vector3(transformationMatrix[1]);
		}
		
		Vector3 Forward() const
		{
			auto transformationMatrix = GetTransformationMatrix();
			return Vector3(transformationMatrix[2]);
		}

		Matrix4
		GetTransformationMatrix() const;
		
	private:
		Vector3 m_position = Vector3(0);
		Vector3 m_rotation = Vector3(0); // TODO: Convert to quaternion once implemented
		Vector3 m_scale    = Vector3(1);

		EntityId m_parent = EntityId::Null;

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
