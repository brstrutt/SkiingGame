#include "OGLSkybox.h"

#include <windows.h>
#include "GLEW/include/glew.h"

OGLSkybox::OGLSkybox()
{
}


OGLSkybox::~OGLSkybox()
{
}

void OGLSkybox::Init()
{
	SimpleVertex corners[] =
	{
		{ -50.0f, -50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },// 0
		{ 50.0f, -50.0f, 50.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f }, // 1
		{ 50.0f, 50.0f, 50.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f }, // 2
		{ -50.0f, 50.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 3

		{ 50.0f, -50.0f, 50.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // 4
		{ 50.0f, -50.0f, -50.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, // 5
		{ 50.0f, 50.0f, -50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }, // 6
		{ 50.0f, 50.0f, 50.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 7

		{ 50.0f, -50.0f, -50.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f }, // 8
		{ -50.0f, -50.0f, -50.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }, //9
		{ -50.0f, 50.0f, -50.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }, // 10
		{ 50.0f, 50.0f, -50.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 11

		{ -50.0f, -50.0f, -50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }, //12
		{ -50.0f, -50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },// 13
		{ -50.0f, 50.0f, 50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f }, // 14
		{ -50.0f, 50.0f, -50.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 15

		{ -50.0f, 50.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }, // 16
		{ 50.0f, 50.0f, 50.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f }, // 17
		{ 50.0f, 50.0f, -50.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }, // 18
		{ -50.0f, 50.0f, -50.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 19

		{ -50.0f, -50.0f, -50.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }, //20
		{ 50.0f, -50.0f, -50.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, // 21
		{ 50.0f, -50.0f, 50.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // 22
		{ -50.0f, -50.0f, 50.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }// 23
	};

	//We will use triangles instead of triangle strips
	unsigned int indices[] =
	{
		3, 1, 0,
		3, 2, 1,

		7, 5, 4,
		7, 6, 5,

		11, 9, 8,
		11, 10, 9,

		15, 14, 12,
		14, 13, 12,

		19, 18, 16,
		18, 17, 16,

		23, 22, 20,
		22, 21, 20
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(SimpleVertex), corners, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 12));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 24));
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &m_vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	for (int i = 0; i < 6; i++)
	{
		m_textures[i] = new OGLTexture();
	}

	m_textures[1]->CreateTextureFromFile("../asset/texture/Skybox/Snowy/sky_rt.tga");
	m_textures[2]->CreateTextureFromFile("../asset/texture/Skybox/Snowy/sky_bk.tga");
	m_textures[3]->CreateTextureFromFile("../asset/texture/Skybox/Snowy/sky_lt.tga");
	m_textures[0]->CreateTextureFromFile("../asset/texture/Skybox/Snowy/sky_ft.tga");
	m_textures[4]->CreateTextureFromFile("../asset/texture/Skybox/Snowy/sky_tp.tga");
	m_textures[5]->CreateTextureFromFile("../asset/texture/Skybox/Snowy/sky_bt.tga");
}

void OGLSkybox::Render()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glBindVertexArray(m_vao);

	glActiveTexture(GL_TEXTURE0);

	for (int i = 0; i < 6; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_textures[i]->m_syshandle);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((i * 6) *sizeof(GLuint)));
	}

	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}