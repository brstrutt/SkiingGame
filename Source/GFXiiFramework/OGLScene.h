#pragma once
#include "Renderable.h"
#include "OpenGL\OGLCube.h"
#include "OpenGL\OGLMesh.h"
#include "OpenGL\OGLTexture.h"
#include "OGLSkybox.h"
#include "OGLSpaceship.h"
#include "OGLTerrainManager.h"
#include "camera.h"
#include <list>
#include "OpenGL\OGLShader.h"
#include "OGLMeshCollider.h"
#include "Player.h"
#include "OGLBillboard.h"
#include "PhysicsObject.h"
#include "HUD.h"


class OGLScene
{
private:
	std::list<Renderable*> renderables;
	OGLSkybox* theSkybox;
	OGLTerrainManager* floorTer;

	Renderable* testing;

	OGLSpaceship mover;
	bool moverActive;

	OGLArrow *satPointer, *ufoPointer;
	std::vector<PhysicsObject*> physics;

	Camera* theCamera;
	Player* thePlayer;

	OGLShaderProgram *skyboxShader, *terrainShader, *meshShader, *billboardShader, *HUDShader;
	
	HUD* theHUD;

	void CreateCollidable(RenderableObject<OGLMesh>* objectToAdd);
	void LoopSats();

public:
	OGLScene();
	~OGLScene();
	
	void CreateScene();
	void AddRenderable(Renderable* renderableToAdd);

	void Update();
	void Render();

private:

	void RendSky();
	void RendMesh();
	void RendTerr();
	void RendBill();
	void RendHUD();

public:

	OGLTerrainManager* GetTerrain()
	{
		return floorTer;
	}

	void Remove(Renderable* renderableToRemove)
	{
		delete(renderableToRemove);
		renderables.remove(renderableToRemove);
	}

	void SetShaderPrograms(OGLShaderProgram* s_shader, OGLShaderProgram* t_shader, OGLShaderProgram* m_shader, OGLShaderProgram* b_shader, OGLShaderProgram* HUD_shader)
	{
		skyboxShader = s_shader;
		terrainShader = t_shader;
		meshShader = m_shader;
		billboardShader = b_shader;
		HUDShader = HUD_shader;
	}

	void SetCamera(Camera* newCam)
	{
		theCamera = newCam;
		//LoopSats();
		mover.SetPlayerPosition(newCam->GetCamPos());
	}

	void SetPlayer(Player* newPl)
	{
		thePlayer = newPl;
	}

	void CreateHUD(int* widthPntr, int* heightPntr)
	{
		theHUD = new HUD(thePlayer);
		theHUD->SetScreenSize(heightPntr, widthPntr);
	}
};
