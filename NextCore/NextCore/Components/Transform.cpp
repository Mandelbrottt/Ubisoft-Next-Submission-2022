#include "pch.h"

#include "Transform.h"

#include "Math/Transformations.h"

namespace Next
{
	ReflectRegister(Transform);

	Transform*
	Transform::GetParent()
	{
		throw "Not Implemented";
	}

	Transform const*
	Transform::GetParent() const
	{
		throw "Not Implemented";
	}

	EntityId
	Transform::GetParentId() const
	{
		throw "Not Implemented";
	}

	void
	Transform::SetParent(Transform* a_parent)
	{
		throw "Not Implemented";
	}

	void
	Transform::SetParent(EntityId a_parent)
	{
		throw "Not Implemented";
	}

	Matrix4
	Transform::GetTransformationMatrix() const
	{
		if (m_isMatrixDirty)
		{
			// Calculate New Matrix
			Matrix4 result = Matrix4::Identity();
			result *= Matrix::Scale(m_scale);
			result *= Matrix::RotateZ(m_rotation.z);
			result *= Matrix::RotateY(m_rotation.y);
			result *= Matrix::RotateX(m_rotation.x);
			result *= Matrix::Translate(m_position);

			// Implement parenting
			//result *= GetParent()->GetTransformationMatrix();

			m_cachedTransformationMatrix = std::move(result);
			m_isMatrixDirty = false;
		}

		/* TODO: Get parent matrix. Find way to recalculate this matrix if parent was made dirty
		         even if it has been marked as not dirty before this calculation is done.
		         Events / function pointers? */
		Matrix4 parent = Matrix4::Identity();

		return parent * m_cachedTransformationMatrix;
	}
}
