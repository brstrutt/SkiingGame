#pragma once
#include "Renderable.h"
class OGLBillboard : public Renderable
{
protected:
	unsigned int	m_vao;
	unsigned int	m_vbo_verts;

	float width, height;
	float UVX, UVY;
	
public:
	OGLBillboard();
	OGLBillboard(const char* texName);
	~OGLBillboard();

	virtual void CreateBoard();

	void Render();

	void SetSize(float newWidth, float newHeight)
	{
		width = newWidth;
		height = newHeight;
	}

	void SetUVs(float newUVX, float newUVY)
	{
		UVX = newUVX;
		UVY = newUVY;
	}
};

class OGLArrow : public OGLBillboard
{
public: 
	OGLArrow();
	OGLArrow(const char* texName);
	~OGLArrow();

	void CreateBoard();
};

