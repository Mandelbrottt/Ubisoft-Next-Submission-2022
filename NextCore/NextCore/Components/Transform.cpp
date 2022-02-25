#include "pch.h"

#include "Transform.h"

#include "Math/Transformations.h"

namespace Next
{
	REFLECT_REGISTER(Transform);

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
		// TEMPORARY:
		m_isMatrixDirty = true;

		if (m_isMatrixDirty)
		{
			// Calculate New Matrix
			Matrix4 result = Matrix4::Identity();
			result *= Matrix::Scale(m_scale);
			result *= Matrix::RotateZ(m_rotation.z);
			result *= Matrix::RotateY(m_rotation.y);
			result *= Matrix::RotateX(m_rotation.x);
			result *= Matrix::Translate(m_position);

			// TODO: Implement parenting
			//result *= GetParent()->GetTransformationMatrix();

			m_cachedTransformationMatrix = std::move(result);
			m_isMatrixDirty = false;
		}

		return m_cachedTransformationMatrix;
	}
}
