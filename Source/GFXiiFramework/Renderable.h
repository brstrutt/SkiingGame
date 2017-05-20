#pragma once

#include "BaseTexture.h"
#include "Vector3.h"
#include "matrix4x4.h"

//This is an abstract class
//It cannot be instantiated without being inherited by a concrete class
class Renderable
{
protected:
	BaseTexture *m_tex1, *m_tex2;
	Vector3 m_location;
	Vector3 m_rotation;
	float scale;

public:

	//this is a pure virtual function
	Renderable()
	{
		m_tex1 = 0;
		m_location.SetVector(0, 0, 0);
		m_rotation.SetVector(0, 0, 0);
		scale = 1;
	}
	virtual void Render() = 0;

	inline void SetTexture(bool mainTex, BaseTexture* tex)
	{
		if (mainTex) m_tex1 = tex;
		else m_tex2 = tex;
	}

	void SetLocation(float x, float y, float z)
	{
		SetLocation(Vector3(x, y, z));
	}

	void SetLocation(Vector3 location)
	{
		m_location = location;
	}

	void SetRotation(float x, float y, float z)
	{
		SetRotation(Vector3(x, y, z));
	}

	void SetRotation(Vector3 rotation)
	{
		m_rotation = rotation;
	}

	void SetScale(float newScale)
	{
		scale = newScale;
	}

	float GetScale()
	{
		return scale;
	}

	Vector3 GetRotation()
	{
		return m_rotation;
	}

	Vector3 GetLocation()
	{
		return m_location;
	}

	virtual Matrix4x4 GetModelMatrix();
};

template <class a_type> class RenderableObject
{
private:
	a_type* object;

	Vector3 m_location;
	Vector3 m_rotation;
	float scale;

public:

	RenderableObject()
	{
		m_location.SetVector(0, 0, 0);
		m_rotation.SetVector(0, 0, 0);
		scale = 1;
	}

	~RenderableObject(){}
	
	void SetRenderable(a_type* newRenderable)
	{
		object = newRenderable;
	}

	a_type* GetRenderable()
	{
		return object;
	}

	void SetLocation(float x, float y, float z)
	{
		SetLocation(Vector3(x, y, z));
	}

	void SetLocation(Vector3 location)
	{
		m_location = location;
	}

	void SetRotation(float x, float y, float z)
	{
		SetRotation(Vector3(x, y, z));
	}

	void SetRotation(Vector3 rotation)
	{
		m_rotation = rotation;
	}

	void SetScale(float newScale)
	{
		scale = newScale;
	}

	float GetScale()
	{
		return scale;
	}

	Vector3 GetRotation()
	{
		return m_rotation;
	}

	Vector3 GetLocation()
	{
		return m_location;
	}

	Matrix4x4 GetModelMatrix()
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
};