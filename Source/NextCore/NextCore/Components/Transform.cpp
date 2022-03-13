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

	Vector3 const&
	Transform::GetLocalPosition() const
	{
		return m_position;
	}

	Vector3
	Transform::GetPosition() const
	{
		if (m_parent == nullptr)
		{
			return GetLocalPosition();
		}

		Vector4 multResult = Vector4(m_position, 1.0f);
		multResult         = multResult * m_parent->GetTransformationMatrix();

		return Vector3(multResult);
	}

	void
	Transform::SetLocalPosition(Vector3 const a_position)
	{
		m_isMatrixDirty |= a_position != m_position;

		m_position = a_position;
	}

	void
	Transform::SetPosition(Vector3 const a_position)
	{
		m_isMatrixDirty = true;

		if (m_parent == nullptr)
		{
			return SetLocalPosition(a_position);
		}

		Vector4 multResult = Vector4(a_position, 1.0f);
		multResult         = multResult * Matrix::ViewInverse(m_parent->GetTransformationMatrix());

		m_position = Vector3(multResult);
	}

	Matrix3 const&
	Transform::GetLocalRotation() const
	{
		return m_rotation;
	}

	Vector3
	Transform::GetLocalRotationEulerAngles() const
	{
		return Matrix::EulerAngles(Matrix3(GetTransformationMatrix(false)));
	}

	Vector3
	Transform::GetRotationEulerAngles() const
	{
		return Matrix::EulerAngles(Matrix3(GetTransformationMatrix()));
	}

	Matrix3 const&
	Transform::SetLocalRotation(Vector3 const a_rotation)
	{
		m_isMatrixDirty = true;

		Matrix4 result = Matrix4::Identity();
		result *= Matrix::RotateX(-a_rotation.x); // Negate rotation so that +x rotates up, +y right, +z clockwise
		result *= Matrix::RotateY(-a_rotation.y); // Not sure why these values need to be negated to get the
		result *= Matrix::RotateZ(-a_rotation.z); // result we want

		return m_rotation = Matrix3(result);
	}

	Matrix3 const&
	Transform::SetLocalRotation(Matrix3 const a_rotation)
	{
		m_isMatrixDirty = true;

		return m_rotation = a_rotation;
	}

	Matrix3 const&
	Transform::SetRotation(Matrix3 const a_rotation)
	{
		m_isMatrixDirty = true;

		if (m_parent == nullptr)
		{
			return SetLocalRotation(a_rotation);
		}

		Matrix4 multResult = Matrix4(a_rotation);
		multResult[3][3]   = 1.0f;
		multResult         = multResult * Matrix::ViewInverse(m_parent->GetTransformationMatrix());

		return m_rotation = Matrix3(multResult);
	}

	Vector3 const&
	Transform::GetLocalScale() const
	{
		return m_scale;
	}

	Vector3 const&
	Transform::SetLocalScale(Vector3 const a_scale)
	{
		m_isMatrixDirty |= a_scale == m_scale;

		return m_scale = a_scale;
	}

	Vector3
	Transform::Right() const
	{
		auto transformationMatrix = GetTransformationMatrix();
		return Vector3(transformationMatrix[0]);
	}

	Vector3
	Transform::Up() const
	{
		auto transformationMatrix = GetTransformationMatrix();
		return Vector3(transformationMatrix[1]);
	}

	Vector3
	Transform::Forward() const
	{
		auto transformationMatrix = GetTransformationMatrix();
		return Vector3(transformationMatrix[2]);
	}

	void
	Transform::LookAt(Vector3 a_target, Vector3 a_up)
	{
		Matrix3 lookAt = Matrix3(Matrix::LookAt(GetPosition(), a_target, a_up));

		SetRotation(lookAt);
	}

	Matrix4
	Transform::GetTransformationMatrix(bool a_includeParent) const
	{
		if (m_isMatrixDirty)
		{
			// Calculate New Matrix
			Matrix4 result = Matrix4(m_rotation);
			result[0] *= m_scale.x;
			result[1] *= m_scale.y;
			result[2] *= m_scale.z;
			//result *= Matrix::Scale(m_scale);
			//result *= Matrix::RotateX(-m_rotation.x); // Negate rotation so that +x rotates up, +y right, +z clockwise
			//result *= Matrix::RotateY(-m_rotation.y); // Not sure why these values need to be negated to get the
			//result *= Matrix::RotateZ(-m_rotation.z); // result we want
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
