#include "matrix4x4.h"


Matrix4x4::Matrix4x4()
{
	float temp_Matrix[16] = 
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f 
	};

	SetMatrix(temp_Matrix);
}


Matrix4x4::~Matrix4x4()
{
}

void Matrix4x4::SetMatrix(const float* newMatrix)
{
	for (int i = 0; i < 16; i++)
	{
		m_data[i] = newMatrix[i];
	}
}

void Matrix4x4::SetMatrix(	float val00, float val01, float val02, float val03,
							float val10, float val11, float val12, float val13,
							float val20, float val21, float val22, float val23,
							float val30, float val31, float val32, float val33)
{
	float temp_Matrix[16] =
	{
		val00, val01, val02, val03,
		val10, val11, val12, val13,
		val20, val21, val22, val23,
		val30, val31, val32, val33
	};

	SetMatrix(temp_Matrix);
}

Matrix4x4 Matrix4x4::operator * (const Matrix4x4& rhs) const
{
	Matrix4x4 resultMat;
	float resultArr[16];

	resultArr[0] = (m_data[0] * rhs.GetMatrix()[0]) + (m_data[1] * rhs.GetMatrix()[4]) + (m_data[2] * rhs.GetMatrix()[8]) + (m_data[3] * rhs.GetMatrix()[12]);
	resultArr[1] = (m_data[0] * rhs.GetMatrix()[1]) + (m_data[1] * rhs.GetMatrix()[5]) + (m_data[2] * rhs.GetMatrix()[9]) + (m_data[3] * rhs.GetMatrix()[13]);
	resultArr[2] = (m_data[0] * rhs.GetMatrix()[2]) + (m_data[1] * rhs.GetMatrix()[6]) + (m_data[2] * rhs.GetMatrix()[10]) + (m_data[3] * rhs.GetMatrix()[14]);
	resultArr[3] = (m_data[0] * rhs.GetMatrix()[3]) + (m_data[1] * rhs.GetMatrix()[7]) + (m_data[2] * rhs.GetMatrix()[11]) + (m_data[3] * rhs.GetMatrix()[15]);

	resultArr[4] = (m_data[4] * rhs.GetMatrix()[0]) + (m_data[5] * rhs.GetMatrix()[4]) + (m_data[6] * rhs.GetMatrix()[8]) + (m_data[7] * rhs.GetMatrix()[12]);
	resultArr[5] = (m_data[4] * rhs.GetMatrix()[1]) + (m_data[5] * rhs.GetMatrix()[5]) + (m_data[6] * rhs.GetMatrix()[9]) + (m_data[7] * rhs.GetMatrix()[13]);
	resultArr[6] = (m_data[4] * rhs.GetMatrix()[2]) + (m_data[5] * rhs.GetMatrix()[6]) + (m_data[6] * rhs.GetMatrix()[10]) + (m_data[7] * rhs.GetMatrix()[14]);
	resultArr[7] = (m_data[4] * rhs.GetMatrix()[3]) + (m_data[5] * rhs.GetMatrix()[7]) + (m_data[6] * rhs.GetMatrix()[11]) + (m_data[7] * rhs.GetMatrix()[15]);

	resultArr[8] = (m_data[8] * rhs.GetMatrix()[0]) + (m_data[9] * rhs.GetMatrix()[4]) + (m_data[10] * rhs.GetMatrix()[8]) + (m_data[11] * rhs.GetMatrix()[12]);
	resultArr[9] = (m_data[8] * rhs.GetMatrix()[1]) + (m_data[9] * rhs.GetMatrix()[5]) + (m_data[10] * rhs.GetMatrix()[9]) + (m_data[11] * rhs.GetMatrix()[13]);
	resultArr[10] = (m_data[8] * rhs.GetMatrix()[2]) + (m_data[9] * rhs.GetMatrix()[6]) + (m_data[10] * rhs.GetMatrix()[10]) + (m_data[11] * rhs.GetMatrix()[14]);
	resultArr[11] = (m_data[8] * rhs.GetMatrix()[3]) + (m_data[9] * rhs.GetMatrix()[7]) + (m_data[10] * rhs.GetMatrix()[11]) + (m_data[11] * rhs.GetMatrix()[15]);

	resultArr[12] = (m_data[12] * rhs.GetMatrix()[0]) + (m_data[13] * rhs.GetMatrix()[4]) + (m_data[14] * rhs.GetMatrix()[8]) + (m_data[15] * rhs.GetMatrix()[12]);
	resultArr[13] = (m_data[12] * rhs.GetMatrix()[1]) + (m_data[13] * rhs.GetMatrix()[5]) + (m_data[14] * rhs.GetMatrix()[9]) + (m_data[15] * rhs.GetMatrix()[13]);
	resultArr[14] = (m_data[12] * rhs.GetMatrix()[2]) + (m_data[13] * rhs.GetMatrix()[6]) + (m_data[14] * rhs.GetMatrix()[10]) + (m_data[15] * rhs.GetMatrix()[14]);
	resultArr[15] = (m_data[12] * rhs.GetMatrix()[3]) + (m_data[13] * rhs.GetMatrix()[7]) + (m_data[14] * rhs.GetMatrix()[11]) + (m_data[15] * rhs.GetMatrix()[15]);

	resultMat.SetMatrix(resultArr);

	return resultMat;
}