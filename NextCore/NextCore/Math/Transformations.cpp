#include "pch.h"
#include "Transformations.h"

#include "Vector4.h"

namespace Next::Matrix
{
	Matrix4 Perspective(float a_fieldOfView, float a_aspectRatio, float a_nearPlane, float a_farPlane)
	{
		Matrix4 result { 0 };
		
		float fovRadians = 1.f / std::tan(a_fieldOfView * 0.5f / 180.f * PI);

		float verticalAspectRatio = 1 / a_aspectRatio;
		
		result[0][0] = verticalAspectRatio * fovRadians;
		result[1][1] = fovRadians;
		result[2][2] = a_farPlane / (a_farPlane - a_nearPlane);
		result[3][2] = (-a_farPlane * a_nearPlane) / (a_farPlane - a_nearPlane);
		result[2][3] = 1;
		
		return result;
	}

	Matrix4
	RotateX(float a_angle)
	{
		Matrix4 result;

		result[0][0] = 1.0f;
		result[1][1] =  std::cos(a_angle);
		result[1][2] =  std::sin(a_angle);
		result[2][1] = -std::sin(a_angle);
		result[2][2] =  std::cos(a_angle);
		result[3][3] = 1.0f;

		return result;
	}

	Matrix4
	RotateY(float a_angle)
	{
		Matrix4 result;

		result[0][0] =  std::cos(a_angle);
		result[0][2] =  std::sin(a_angle);
		result[1][1] = 1.0f;
		result[2][0] = -std::sin(a_angle);
		result[2][2] =  std::cos(a_angle);
		result[3][3] = 1.0f;

		return result;
	}

	Matrix4
	RotateZ(float a_angle)
	{
		Matrix4 result;

		result[0][0] =  std::cos(a_angle);
		result[0][1] =  std::sin(a_angle);
		result[1][0] = -std::sin(a_angle);
		result[1][1] =  std::cos(a_angle);
		result[2][2] = 1.0f;
		result[3][3] = 1.0f;

		return result;
	}

	Matrix4
	Scale(Vector3 a_scale)
	{
		Matrix4 result;

		result[0][0] = a_scale.x;
		result[1][1] = a_scale.y;
		result[2][2] = a_scale.z;
		result[3][3] = 1.0f;
		
		return result;
	}

	Matrix4
	Translate(Vector3 a_position)
	{
		Matrix4 result = Matrix4::Identity();

		result[3][0] = a_position.x;
		result[3][1] = a_position.y;
		result[3][2] = a_position.z;
		
		return result;
	}
}
