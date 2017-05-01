#pragma once
#include "OpenGL\OGLCube.h"
class OGLSkybox : public OGLCube
{
private:
	OGLTexture *m_textures[6];
public:
	OGLSkybox();
	~OGLSkybox();

	void Init();
	void Render();
};

