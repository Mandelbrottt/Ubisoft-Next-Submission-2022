#pragma once

#include "Matrix4.h"
#include "Vector3.h"

#include "NextCoreCommon.h"

namespace NextCore::Math
{
	/**
	 * \param a_fieldOfView Horizontal Field of view in degrees 
	 * \param a_aspectRatio The aspect ratio (x / y) of the screen
	 * \param a_nearPlane The near plane in units
	 * \param a_farPlane The far plane in units
	 * \return A 4x4 row-major perspective matrix
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	Perspective(float a_fieldOfView, float a_aspectRatio, float a_nearPlane, float a_farPlane);
	
	/**
	 * \param a_angle The angle by which to rotate
	 * \return A 4x4 row-major matrix representing a rotation about the x axis in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	RotateX(float a_angle);

	/**
	 * \param a_angle The angle by which to rotate
	 * \return A 4x4 row-major matrix representing a rotation about the y axis in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	RotateY(float a_angle);

	/**
	 * \param a_angle The angle by which to rotate
	 * \return A 4x4 row-major matrix representing a rotation about the z axis in 3D space
	 */
	NEXT_CORE_EXPORT
	extern
	Matrix4
	RotateZ(float a_angle);
	
	/**
	 * \param a_scale The amount by which to scale
	 * \return A 4x4 row-major matrix representing a scaling operation in 3D space
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
}
