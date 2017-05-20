#pragma once
#include "OpenGL/OGLMesh.h"
#include "OGLBillboard.h"
#include "OpenGL/OGLShader.h"
#include <vector>

class OGLTerrain;

struct foliagePoint
{
	Vector3* position;
	int nextUp, nextRight, nextDown, nextLeft;
};

class Foliage
{
	int squareNum;
	float squareSize;

	int foliageNumber;
	RenderableObject<OGLBillboard>* grassBillboard;
	foliagePoint* foliages;

	OGLTerrain* theTerrain;

public:
	Foliage(int newSquareNum, float newSquareSize, OGLTerrain* terrainPtr);
	~Foliage();


	void RenderFoliage(OGLShaderProgram* billShader, Vector3 camPos, Vector3 terrainLocation);

private:
	void GenerateFoliage();
};

