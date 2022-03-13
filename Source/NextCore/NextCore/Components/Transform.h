#pragma once

#include "Math/Matrix3.h"
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
		GetLocalPosition() const;

		/**
		 * \brief Get the world-space position in this transform's object-space
		 */
		Vector3
		GetPosition() const;

		/**
		 * \brief Set the local position in this transform's object-space
		 */
		void
		SetLocalPosition(Vector3 const a_position);

		/**
		 * \brief Set the local position in this transform's object-space
		 */
		void
		SetPosition(Vector3 const a_position);

		/**
		 * \brief Get the local rotation in this transform's object-space
		 */
		Matrix3 const&
		GetLocalRotation() const;

		/**
		 * \brief Get the local rotation in this transform's object-space in euler-angles
		 */
		Vector3
		GetLocalRotationEulerAngles() const;

		/**
		 * \brief Get the world-space rotation in this transform's object-space
		 */
		Vector3
		GetRotationEulerAngles() const;

		/**
		 * \brief Set the local rotation in this transform's object-space
		 */
		Matrix3 const&
		SetLocalRotation(Vector3 const a_rotation);

		/**
		 * \brief Set the local rotation in this transform's object-space
		 */
		Matrix3 const&
		SetLocalRotation(Matrix3 const a_rotation);

		/**
		 * \brief Set the world-space rotation in this transform's object-space
		 */
		Matrix3 const&
		SetRotation(Matrix3 const a_rotation);

		/**
		 * \brief Get the local scale in this transform's object-space
		 */
		Vector3 const&
		GetLocalScale() const;

		/**
		 * \brief Set the local scale in this transform's object-space
		 */
		Vector3 const&
		SetLocalScale(Vector3 const a_scale);

		/**
		 * \brief Get right vector in this transform's object-space
		 */
		Vector3
		Right() const;

		/**
		 * \brief Get up vector in this transform's object-space
		 */
		Vector3
		Up() const;

		/**
		 * \brief Get forward vector in this transform's object-space
		 */
		Vector3
		Forward() const;

		void
		LookAt(Vector3 a_target, Vector3 a_up = Vector3::Up());
		
		/**
		 * \brief Get the model matrix for this transform in the given space.
		 * \param a_worldSpace Whether to include the calculate the matrix in local or world space.
		 * \return The 4x4 row-major matrix that represents this object's model transformation
		 */
		Matrix4
		GetTransformationMatrix(bool a_worldSpace = true) const;
	
	private:
		Vector3 m_position = Vector3(0);
		//Vector3 m_rotation = Vector3(0); // TODO: Convert to quaternion once implemented
		Matrix3 m_rotation = Matrix3::Identity();
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
