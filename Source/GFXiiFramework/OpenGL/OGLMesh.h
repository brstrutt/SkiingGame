#pragma once

#include <Windows.h>
#include "../Renderable.h"
#include "../Triangle.h"


class OGLMesh : public Renderable
{
protected:
	unsigned int	m_vao;
	unsigned int	m_vbo_verts;
	int							m_numtriangles;

public:
	OGLMesh();
	OGLMesh(LPCWSTR filename, const char* texName);
	~OGLMesh();

	void LoadAndBuildMeshFromOBJFile(LPCWSTR filename);

	virtual void Render();

};