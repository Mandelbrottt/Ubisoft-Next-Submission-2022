#include "pch.h"

#include "Transform.h"

#include "Math/Transformations.h"

namespace Next
{
	ReflectRegister(Transform);

	Transform*
	Transform::GetParent()
	{
		return m_parent;
	}

	Transform const*
	Transform::GetParent() const
	{
		return m_parent;
	}
	
	void
	Transform::SetParent(Transform* a_parent)
	{
		m_parent = a_parent;
	}
	
	Matrix4
	Transform::GetTransformationMatrix() const
	{
		if (m_isMatrixDirty)
		{
			// Calculate New Matrix
			Matrix4 result = Matrix4::Identity();
			result *= Matrix::Scale(m_scale);
			result *= Matrix::RotateX(-m_rotation.x); // Negate rotation so that +x rotates up, +y right, +z clockwise
			result *= Matrix::RotateY(-m_rotation.y); // Not sure why these values need to be negated to get the
			result *= Matrix::RotateZ(-m_rotation.z); // result we want
			result *= Matrix::Translate(m_position);

			// Implement parenting
			//result *= GetParent()->GetTransformationMatrix();

			m_cachedTransformationMatrix = std::move(result);
			m_isMatrixDirty = false;
		}

		/* TODO: Get parent matrix. Find way to recalculate this matrix if parent was made dirty
		         even if it has been marked as not dirty before this calculation is done.
		         Events / function pointers? */
		Matrix4 parent = m_parent == nullptr ? Matrix4::Identity() : m_parent->GetTransformationMatrix();
		
		return m_cachedTransformationMatrix * parent;
	}
}
