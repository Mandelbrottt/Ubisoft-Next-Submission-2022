#pragma once

#include "Math/Matrix4.h"
#include "Math/Transformations.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"

#include "Scripting/Component.h"

namespace Next
{
	class Transform : public Component
	{
		ComponentDeclare(Transform, Component)
			
	public:
		Transform*
		GetParent();
		
		Transform const*
		GetParent() const;
		
		void
		SetParent(Transform* a_parent);
		
		/**
		 * \brief Get the local position in this transform's object-space
		 */
		Vector3 const&
		GetLocalPosition() const
		{
			return m_position;
		}
		
		/**
		 * \brief Get the world-space position in this transform's object-space
		 */
		Vector3
		GetPosition() const
		{
			if (m_parent == nullptr)
			{
				return GetLocalPosition();
			}

			Vector4 multResult = Vector4(m_position, 1.0f);
			multResult = multResult * m_parent->GetTransformationMatrix();
			
			return Vector3(multResult);
		}
		
		/**
		 * \brief Set the local position in this transform's object-space
		 */
		void
		SetLocalPosition(Vector3 const a_position)
		{
			m_isMatrixDirty |= a_position != m_position;
			
			m_position = a_position;
		}
		
		/**
		 * \brief Set the local position in this transform's object-space
		 */
		void
		SetPosition(Vector3 const a_position)
		{
			m_isMatrixDirty = true;

			if (m_parent == nullptr)
			{
				return SetLocalPosition(a_position);
			}

			Vector4 multResult = Vector4(a_position, 1.0f);
			multResult = multResult * Matrix::ViewInverse(m_parent->GetTransformationMatrix());
			
			m_position = Vector3(multResult);
		}
		
		/**
		 * \brief Get the local rotation in this transform's object-space
		 */
		Vector3 const&
		GetLocalRotation() const
		{
			return m_rotation;
		}
		
		/**
		 * \brief Get the world-space rotation in this transform's object-space
		 */
		Vector3
		GetRotation() const
		{
			return Matrix::EulerAngles(GetTransformationMatrix());
		}
		
		/**
		 * \brief Set the local rotation in this transform's object-space
		 */
		Vector3 const&
		SetLocalRotation(Vector3 const a_rotation)
		{
			m_isMatrixDirty |= a_rotation != m_rotation;
			
			return m_rotation = a_rotation;
		}
		
		///**
		// * \brief Set the world-space rotation in this transform's object-space
		// */
		//Vector3
		//SetRotation(Vector3 const a_rotation)
		//{
		//	m_isMatrixDirty = true;
		//	
		//	return m_rotation = a_rotation;
		//}
		
		/**
		 * \brief Get the local scale in this transform's object-space
		 */
		Vector3 const&
		GetLocalScale() const
		{
			return m_scale;
		}
		
		/**
		 * \brief Set the local scale in this transform's object-space
		 */
		Vector3 const&
		SetLocalScale(Vector3 const a_scale)
		{
			m_isMatrixDirty |= a_scale == m_scale;
			
			return m_scale = a_scale;
		}
		
		/**
		 * \brief Get right vector in this transform's object-space
		 */
		Vector3 Right() const
		{
			auto transformationMatrix = GetTransformationMatrix();
			return Vector3(transformationMatrix[0]);
		}
		
		/**
		 * \brief Get up vector in this transform's object-space
		 */
		Vector3 Up() const
		{
			auto transformationMatrix = GetTransformationMatrix();
			return Vector3(transformationMatrix[1]);
		}
		
		/**
		 * \brief Get forward vector in this transform's object-space
		 */
		Vector3 Forward() const
		{
			auto transformationMatrix = GetTransformationMatrix();
			return Vector3(transformationMatrix[2]);
		}

		//void
		//LookAt(Vector3 a_target, Vector3 a_up = Vector3::Up());
		//
		
		/**
		 * \brief Get the model matrix for this transform in the given space.
		 * \param a_worldSpace Whether to include the calculate the matrix in local or world space.
		 * \return The 4x4 row-major matrix that represents this object's model transformation
		 */
		Matrix4
		GetTransformationMatrix(bool a_worldSpace = true) const;
	
	private:
		Vector3 m_position = Vector3(0);
		Vector3 m_rotation = Vector3(0); // TODO: Convert to quaternion once implemented
		Vector3 m_scale    = Vector3(1);
		
		Transform* m_parent = nullptr;

		mutable Matrix4 m_cachedTransformationMatrix;

		mutable bool m_isMatrixDirty = true;
		
		ReflectMembers(
			ReflectField(m_position)
			ReflectField(m_rotation)
			ReflectField(m_scale)
			ReflectField(m_parent)
		)
	};
}
