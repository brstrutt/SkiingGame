#include "OGLWindow.h"
#include "Resource.h"
#include "GLEW/include/glew.h"
#include "matrix4x4.h"


OGLWindow::OGLWindow()
{
}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete m_shader;

	DestroyOGLContext();
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyRenderWindow()
{
	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{

	glDeleteSamplers( 1, (GLuint*)(&m_texDefaultSampler) );

	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"RenderWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd ) return FALSE;

	m_hdc = GetDC( m_hwnd );

	if ( !(m_hglrc = CreateOGLContext( m_hdc )) ) return FALSE;

	glewInit();

	InitOGLState();

	m_width = width;
	m_height = height;

	m_scene.CreateScene();	

	m_camera.InitCamera();
	m_camera.SetCameraAspectRatio(m_width / m_height);
	m_camera.SetCameraFOV(7.9);

	m_camera.UpdateProjectionMatrix();
	playerChar = Player(&m_camera, m_scene.GetTerrain());

	m_scene.SetPlayer(&playerChar);
	m_scene.CreateHUD(&m_width, &m_height);
	m_scene.SetShaderPrograms(m_skybox_shader, m_terrain_shader, m_shader, m_billboard_shader, m_hud_shader);
	m_scene.SetCamera(&m_camera);

	return TRUE;
}

void OGLWindow::Render()
{	
	m_scene.Update();
	playerChar.Update();	

	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();	

	glBindSampler(0, m_texDefaultSampler);	
	
	m_scene.Render();

	//Update screen
	SwapBuffers(m_hdc);

	return;
}

void OGLWindow::Resize( int width, int height )
{
	m_camera.SetCameraAspectRatio((float)width / (float)height);
	
	glViewport( 0, 0, width, height );
	
	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	
	glEnable(GL_CULL_FACE);	

	//Initialise OGL shader
	m_shader = new OGLShaderProgram();

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/testing.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/testing.frag", SHADER_FRAGMENT);

	m_shader->BindAttributeLocation( 0, "position" );
	m_shader->BindAttributeLocation( 1, "inNormal" );
	m_shader->BindAttributeLocation( 2, "inUV" );

	glBindFragDataLocation( m_shader->GetProgramHandle(), 0, "outFrag" );

	m_shader->BuildShaderProgram();




	//Initialise skybox shader
	m_skybox_shader = new OGLShaderProgram();

	m_skybox_shader->CreateShaderProgram();
	m_skybox_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/skybox.vert", SHADER_VERTEX);
	m_skybox_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/skybox.frag", SHADER_FRAGMENT);

	m_skybox_shader->BindAttributeLocation(0, "position");
	m_skybox_shader->BindAttributeLocation(1, "inNormal");
	m_skybox_shader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(m_skybox_shader->GetProgramHandle(), 0, "outFrag");

	m_skybox_shader->BuildShaderProgram();
	



	m_terrain_shader = new OGLShaderProgram();

	m_terrain_shader->CreateShaderProgram();
	m_terrain_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/terrain.vert", SHADER_VERTEX);
	m_terrain_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/terrain.frag", SHADER_FRAGMENT);

	m_terrain_shader->BindAttributeLocation(0, "position");
	m_terrain_shader->BindAttributeLocation(1, "inNormal");
	m_terrain_shader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(m_terrain_shader->GetProgramHandle(), 0, "outFrag");

	m_terrain_shader->BuildShaderProgram();




	m_billboard_shader = new OGLShaderProgram();

	m_billboard_shader->CreateShaderProgram();
	m_billboard_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/billboard.vert", SHADER_VERTEX);
	m_billboard_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/billboard.frag", SHADER_FRAGMENT);

	m_billboard_shader->BindAttributeLocation(0, "position");
	m_billboard_shader->BindAttributeLocation(1, "inNormal");
	m_billboard_shader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(m_billboard_shader->GetProgramHandle(), 0, "outFrag");

	m_billboard_shader->BuildShaderProgram();



	m_hud_shader = new OGLShaderProgram();

	m_hud_shader->CreateShaderProgram();
	m_hud_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/HUD.vert", SHADER_VERTEX);
	m_hud_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/HUD.frag", SHADER_FRAGMENT);

	m_hud_shader->BindAttributeLocation(0, "position");
	m_hud_shader->BindAttributeLocation(1, "inNormal");
	m_hud_shader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(m_hud_shader->GetProgramHandle(), 0, "outFrag");

	m_hud_shader->BuildShaderProgram();


	//Create a texture sampler
	glGenSamplers( 1, (GLuint*)(&m_texDefaultSampler) );
	
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glSamplerParameterf(m_texDefaultSampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0);
}

BOOL OGLWindow::MouseLBDown ( POINT newLoc )
{
	LMBDown = true;
	mousePnt = newLoc;
	return TRUE;
}

BOOL OGLWindow::MouseLBUp(POINT newLoc)
{
	LMBDown = false;
	mousePnt = newLoc;
	return TRUE;
}

BOOL OGLWindow::MouseMove(POINT newLoc)
{
	if (LMBDown)
	{
		m_camera.RotateCamera(float(mousePnt.x - newLoc.x) / 200.0f, float(mousePnt.y - newLoc.y) / 200.0f, 0);
	}
	mousePnt = newLoc;
	return TRUE;
}

void OGLWindow::MouseWheelMove(float distance)
{
	m_camera.ZoomCamera(distance / -2400.0f);
}

void OGLWindow::CheckInput()
{
	float speed = 1.5;
	
	playerChar.GetInput();

	POINT cursInf;

	GetCursorPos(&cursInf);
	MouseMove(cursInf);

	if (GetAsyncKeyState(VK_LBUTTON))
	{
		MouseLBDown(cursInf);
	}
	else
	{
		MouseLBUp(cursInf);
	}


	if (GetAsyncKeyState(VK_UP))
	{
		m_camera.RotateCamera(0, 0.05, 0);
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		m_camera.RotateCamera(0.05, 0, 0);
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		m_camera.RotateCamera(0, -0.05, 0);
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		m_camera.RotateCamera(-0.05, 0, 0);
	}
}
