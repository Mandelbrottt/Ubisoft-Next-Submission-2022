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
	Transform::GetTransformationMatrix(bool a_includeParent) const
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
			
			m_cachedTransformationMatrix = std::move(result);
			m_isMatrixDirty = false;
		}

		Matrix4 result = m_cachedTransformationMatrix;

		if (a_includeParent && m_parent)
		{
			result *= m_parent->GetTransformationMatrix(a_includeParent);
		}
		
		return result;
	}
}
