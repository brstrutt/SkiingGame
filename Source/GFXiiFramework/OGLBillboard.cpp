#include "OGLBillboard.h"
#include "OpenGL\OGLTexture.h"
#include "GLEW/include/glew.h"
#include "../Triangle.h"


OGLBillboard::OGLBillboard()
{
}

OGLBillboard::OGLBillboard(const char* texName)
{
	OGLTexture* meshTex = new OGLTexture();
	meshTex->CreateTextureFromFile(texName);
	SetTexture(true, meshTex);		
	SetTexture(false, meshTex);

	height = 10;
	width = 10;

	UVX = 1;
	UVY = 1;
}


OGLBillboard::~OGLBillboard()
{
}

void OGLBillboard::CreateBoard()
{
	//create the grid of triangles
	Triangle* grid = new Triangle[2];
	
	grid[0].SetVertices(Vector3(width / -2, height, 0), Vector3(width / -2, 0, 0), Vector3(width / 2, 0, 0));
	grid[0].SetNormals(Vector3(-1, 0, 0), Vector3(-1, 0, 0), Vector3(-1, 0, 0));
	grid[0].SetTexCoords(Vector3(0, UVY, 0), Vector3(0, 0, 0), Vector3(UVX, 0, 0));

	grid[1].SetVertices(Vector3(width / -2, height, 0), Vector3(width / 2, 0, 0), Vector3(width / 2, height, 0));
	grid[1].SetNormals(Vector3(-1, 0, 0), Vector3(-1, 0, 0), Vector3(-1, 0, 0));
	grid[1].SetTexCoords(Vector3(0, UVY, 0), Vector3(UVX, 0, 0), Vector3(UVX, UVY, 0));

	int offset = sizeof(Vector3);
	int stride = sizeof(Vertex);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 6 * stride, &(grid[0].m_vertices[0].m_position[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(NULL + offset));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(NULL + 2 * offset));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	delete[] grid;
}

void OGLBillboard::Render()
{
	unsigned int texHandle = dynamic_cast<OGLTexture*>(m_tex1)->m_syshandle;
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);

	unsigned int texHandle2 = dynamic_cast<OGLTexture*>(m_tex2)->m_syshandle;

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texHandle2);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glDrawArrays(GL_TRIANGLES, 0, 12);

	glBindVertexArray(0);
}

OGLArrow::OGLArrow()
{
}

OGLArrow::OGLArrow(const char* texName) : OGLBillboard(texName)
{
	CreateBoard();

	//OGLTexture* meshTex = new OGLTexture();
	//meshTex->CreateTextureFromFile(texName);
	//SetTexture(true, meshTex);
}

OGLArrow::~OGLArrow()
{
}

void OGLArrow::CreateBoard()
{
	float width, height;

	width = 10;
	height = 10;

	//create the grid of triangles
	Triangle* grid = new Triangle[2];

	grid[0].SetVertices(Vector3(width / -2, height, 0), Vector3(width / 2, height, 0), Vector3(0, 0, 0));
	grid[0].SetNormals(Vector3(0, 0, 1), Vector3(0, 0, 1), Vector3(0, 0, 1));
	grid[0].SetTexCoords(Vector3(0, 1, 0), Vector3(1, 0, 0), Vector3(0, 0, 0));

	grid[1].SetVertices(Vector3(width / -2, height, 0), Vector3(0, 0, 0), Vector3(width / 2, height, 0));
	grid[1].SetNormals(Vector3(0, 0, 1), Vector3(0, 0, 1), Vector3(0, 0, 1));
	grid[1].SetTexCoords(Vector3(0, 1, 0), Vector3(1, 1, 0), Vector3(1, 0, 0));

	int offset = sizeof(Vector3);
	int stride = sizeof(Vertex);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 6 * stride, &(grid[0].m_vertices[0].m_position[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(NULL + offset));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(NULL + 2 * offset));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	delete[] grid;
}
