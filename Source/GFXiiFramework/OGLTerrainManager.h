#pragma once
#include "OGLTerrain.h"
#include "OGLBillboard.h"
#include "OpenGL\OGLShader.h"
#include <list>

class OGLTerrainManager
{
private:
	int seed;
	int squareNum;
	int seaLevel;
	float squareSize;

	Vector3 centreSquareCoords;

	RenderableObject<OGLTerrain>* terr;

	OGLBillboard* theSea;
		
	void FocusOnPoint(Vector3 newCentre);

	void GenerateTerrain();

public:
	OGLTerrainManager();
	OGLTerrainManager(int new_seed, int sq_num, float sq_size);
	~OGLTerrainManager();

	void Update(Vector3 playerPos);

	void Render(int m_uniform_model);
	void RenderSea(int m_uniform_model, int m_uniform_scale);
	void RenderBillboards(OGLShaderProgram* billShader, Vector3 camPos);

	float GetHeightAtPoint(float x, float z);
	Vector3 GetNormalAtPoint(float x, float z);

	Vector3 GetRandomPoint();

	//void AddCollider(OGLMeshCollider* newCollider);
};

