#include "pch.h"

#include "Transform.h"

#include "Math/Transformations.h"

namespace NextCore::Component
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

	Scripting::EntityId
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
	Transform::SetParent(Scripting::EntityId a_parent)
	{
		throw "Not Implemented";
	}

	Math::Matrix4
	Transform::GetTransformationMatrix() const
	{
		using Math::Matrix4;

		// TEMPORARY:
		m_isMatrixDirty = true;

		if (m_isMatrixDirty)
		{
			// Calculate New Matrix
			Matrix4 result = Matrix4::Identity();
			result *= Math::Scale(m_scale);
			result *= Math::RotateZ(m_rotation.z);
			result *= Math::RotateY(m_rotation.y);
			result *= Math::RotateX(m_rotation.x);
			result *= Math::Translate(m_position);

			// TODO: Implement parenting
			//result *= GetParent()->GetTransformationMatrix();

			m_cachedTransformationMatrix = std::move(result);
			m_isMatrixDirty = false;
		}

		return m_cachedTransformationMatrix;
	}
}
