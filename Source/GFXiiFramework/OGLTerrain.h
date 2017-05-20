#pragma once
#include "OpenGL\OGLMesh.h"
#include "OGLBillboard.h"
#include "OpenGL\OGLShader.h"
#include "Foliage.h"
#include <vector>

class OGLTerrain : public OGLMesh
{
private:
	float* heightPoints;
	Vector3* normals;

	int seaLevel;

	int squareNum;
	float squareSize;
	
	Vector3 topLeftPoint;

	Foliage *theGrass;

public:
	OGLTerrain();
	OGLTerrain(const char* texPath, const char* texPath2);
	~OGLTerrain();

	void Render();
	void RenderSea();

	void CreateTerrain(int newSquareNum, float newSquareSize, int seed);
	float* GenerateHeightMap(int squareNum);

	void AveragePoints(float* points, int pointsperRow);
	Vector3* GenerateNormals(float* points, int squareNum, float squareSize);

	float GetHeightAtPoint(float x, float z);
	Vector3 GetNormalAtPoint(float x, float z);

	void RenderFoliage(OGLShaderProgram* billShader, Vector3 camPos, Vector3 terrainLocation);

private:
	int C2To1(int x, int y, int lineLength);
	float Average(float val1, float val2);
	float Barycentric(Vector3 P1, Vector3 P2, Vector3 P3, float givenX, float givenY);
};



