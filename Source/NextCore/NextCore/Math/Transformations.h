#pragma once

#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3.h"

#include "NextCoreCommon.h"

namespace Next::Matrix
{
	/**
	 * \param a_fieldOfView Horizontal Field of view in degrees 
	 * \param a_aspectRatio The vertical aspect ratio (y / x) of the screen
	 * \param a_nearPlane The near plane in units
	 * \param a_farPlane The far plane in units
	 * \return A 4x4 row-major perspective matrix
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	Perspective(float a_fieldOfView, float a_aspectRatio, float a_nearPlane, float a_farPlane);
	
	/**
	 * \param a_angle The angle in degrees by which to rotate
	 * \return A 3x3 row-major matrix representing a rotation about the x axis in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix3
	RotateX(float a_angle);

	/**
	 * \param a_angle The angle in degrees by which to rotate
	 * \return A 3x3 row-major matrix representing a rotation about the y axis in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix3
	RotateY(float a_angle);

	/**
	 * \param a_angle The angle in degrees by which to rotate
	 * \return A 3x3 row-major matrix representing a rotation about the z axis in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix3
	RotateZ(float a_angle);

	/**
	 * \param a_angle The angle in degrees by which to rotate around the axis
	 * \param a_axis The axis around which to rotate
	 * \return A 3x3 row-major matrix representing a rotation about the given axis in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix3
	Rotate(float a_angle, Vector3 a_axis);
	
	/**
	 * \param a_scale The amount by which to scale
	 * \return A 3x3 row-major matrix representing a scaling operation in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	Scale(Vector3 a_scale);

	/**
	 * \param a_position The position by which to translate
	 * \return A 4x4 row-major matrix representing a transformation in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	Translate(Vector3 a_position);

	/**
	 * \brief A quick algorithm for computing the view matrix given a loc-rot matrix
	 * \param a_locRot The Model matrix for the object that represents a camera in 3D space
	 * \return A 4x4 row-major matrix representing an inverse view matrix
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	ViewInverse(Matrix4 a_locRot);

	NEXT_CORE_EXPORT
	extern
	Matrix4
	LookAt(Vector3 a_position, Vector3 a_target, Vector3 a_up = Vector3::Up());

	NEXT_CORE_EXPORT
	extern
	Vector3
	EulerAngles(Matrix3 a_matrix);
}
