#include "Renderable.h"
#include <math.h>

Matrix4x4 Renderable::GetModelMatrix()
{
	Matrix4x4 translationMatrix, rotX, rotY, rotZ;

	translationMatrix.SetMatrix(
		1.0f, 0.0f, 0.0f, m_location[0],
		0.0f, 1.0f, 0.0f, m_location[1],
		0.0f, 0.0f, 1.0f, m_location[2],
		0.0f, 0.0f, 0.0f, 1.0f);

	rotX.SetMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosf(m_rotation[0]), -1 * sinf(m_rotation[0]), 0.0f,
		0.0f, sinf(m_rotation[0]), cosf(m_rotation[0]), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	rotY.SetMatrix(
		cosf(m_rotation[1]), 0.0f, sinf(m_rotation[1]), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-1 * sinf(m_rotation[1]), 0.0f, cosf(m_rotation[1]), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	rotZ.SetMatrix(
		cosf(m_rotation[2]), -1 * sinf(m_rotation[2]), 0.0f, 0.0f,
		sinf(m_rotation[2]), cosf(m_rotation[2]), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	return translationMatrix*(rotX*rotY*rotZ);
}

