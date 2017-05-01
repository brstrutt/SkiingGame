//Created for Graphics I and II
//Author: Minsi Chen

#ifndef __QUATERNION_H__
#define __QUATERNION_H__

#include "vector3.h"
#include "matrix4x4.h"

class Quaternion
{
	private:
		Vector3			m_vector;
		float				m_scalar;

	public:
							Quaternion();
							~Quaternion();

		const Vector3&		GetVector() const;
		const float			GetScalar() const;
		
		float&				operator [] (int index);
		Quaternion			operator + (const Quaternion& rhs) const;
		Quaternion			operator * (const Quaternion& rhs) const;
		Vector3			operator * (const Vector3& rhs) const;
		
		float				Norm();
		float				Norm_Sqr() const;
		void				Normalise();
		void				ConvertToRotationMatrix(Matrix4x4* rot) const;
		void				SetQuaternion(float vx, float vy, float vz, float s);
		void				SetQuaternion(const Vector3& vec, float s);
		void SetQuatByVectors(Vector3 source, Vector3 dest);
};

#endif
