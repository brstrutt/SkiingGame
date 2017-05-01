#pragma once

#include "RenderWindow.h"
#include "camera.h"
#include "OGLShader.h"
#include "OGLScene.h"
#include "Player.h"

class OGLWindow : public RenderWindow
{
	private:
		HDC			m_hdc;				//handle to a device context
		HGLRC		m_hglrc;			//handle to a gl rendering context

		int			m_width;
		int			m_height;
		
		//the scene to be rendered
		OGLScene m_scene;

		//Declear an OGL shader program
		OGLShaderProgram		*m_shader;
		OGLShaderProgram		*m_skybox_shader;
		OGLShaderProgram		*m_terrain_shader;
		OGLShaderProgram		*m_billboard_shader;
		OGLShaderProgram		*m_hud_shader;
		int						m_uniform_texture;
		int						m_texDefaultSampler;

		Camera m_camera;
		Player playerChar;

		//MouseHack
		bool LMBDown;
		POINT mousePnt;

protected:

		HGLRC CreateOGLContext (HDC hdc);
		BOOL DestroyOGLContext();
		void InitOGLState();

	public:
					OGLWindow();
					OGLWindow(HINSTANCE hInstance, int width, int height);
					~OGLWindow();
		
		BOOL		InitWindow(HINSTANCE hInstance, int width, int height);

		void		Render();
		void		Resize( int width, int height );
		void		DestroyRenderWindow();

		BOOL		MouseLBDown(POINT newLoc);
		BOOL		MouseLBUp(POINT newLoc);
		BOOL		MouseMove(POINT newLoc);

		void MouseWheelMove(float distance);

		void CheckInput();
};
