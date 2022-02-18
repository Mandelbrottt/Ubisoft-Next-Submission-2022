#include "pch.h"
#include "Transformations.h"

#include "Vector4.h"

namespace NextCore::Math
{
	Matrix4 Perspective(float a_fieldOfView, float a_aspectRatio, float a_nearPlane, float a_farPlane)
	{
		// Compute near plane dimensions
		//float top = std::tan((a_fieldOfView / 2) * (PI / 180)) * a_nearPlane;
		//float bottom = -top;

		//float right = top * a_aspectRatio;
		//float left = -right;

		Matrix4 result { 0 };

		//result[0][0] = 2 * a_nearPlane / (right - left);

		//result[1][1] = 2 * a_nearPlane / (top - bottom);

		//result[2][0] = (right + left) / (right - left);
		//result[2][1] = (top + bottom) / (top - bottom);
		//result[2][2] = -(a_farPlane + a_nearPlane) / (a_farPlane - a_nearPlane);
		//result[2][3] = -1;

		//result[3][2] = -(2 * a_nearPlane * a_farPlane) / (a_farPlane - a_nearPlane);

		a_fieldOfView = 1.f / tanf(a_fieldOfView * 0.5f / 180.f * PI);

		a_aspectRatio = 1 / a_aspectRatio;
		
		result[0][0] = a_aspectRatio * a_fieldOfView;
		result[1][1] = a_fieldOfView;
		result[2][2] = a_farPlane / (a_farPlane - a_nearPlane);
		result[3][2] = (-a_farPlane * a_nearPlane) / (a_farPlane - a_nearPlane);
		result[2][3] = 1;
		
		return result;
	}

	Matrix4
	Translate(Vector3 a_position)
	{
		Matrix4 result = Matrix4::Identity();
		result[0][3] = a_position.x;
		result[1][3] = a_position.y;
		result[2][3] = a_position.z;
		//result[3] = { a_position, 1.0f };
		
		return result;
	}
}
