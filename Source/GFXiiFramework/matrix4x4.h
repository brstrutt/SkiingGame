#ifndef __MATRIX4X4_H__
#define __MATRIX4X4_H__

#include "vector3.h"

class Matrix4x4
{
private:
	float m_data[16];

public:
	Matrix4x4();
	~Matrix4x4();

	void SetMatrix(const float* newMatrix);
	void SetMatrix(float val00, float val01, float val02, float val03, 
				   float val10, float val11, float val12, float val13, 
				   float val20, float val21, float val22, float val23, 
				   float val30, float val31, float val32, float val33);
	const float* GetMatrix() const
	{
		return m_data;
	}

	Matrix4x4 operator * (const Matrix4x4& rhs) const;

	void SetValue(int index, float newVal)
	{
		m_data[index] = newVal;
	}
};

#endif
