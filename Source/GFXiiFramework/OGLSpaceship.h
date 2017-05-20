#pragma once
#include "OpenGL\OGLMesh.h"
#include "Vector3.h"
#include "PhysicsObject.h"

class OGLSpaceship : public PhysicsObject
{
private:
	OGLMesh* meshToDrop;
	RenderableObject<OGLMesh>* nextToDrop;

	Vector3 nextPoint;

	Vector3* playerPosition;

	void GenerateNextPoint(float playX, float playZ);

public:
	OGLSpaceship();
	OGLSpaceship(OGLTerrainManager* floorTerrain);
	~OGLSpaceship();

	void SetPlayerPosition(Vector3* newPlayerPos)
	{
		playerPosition = newPlayerPos;
	}

	Vector3 GetRotationToPointAt(Vector3 pointAtWhichToPoint);

	void Update();

	RenderableObject<OGLMesh>* CheckDropState()
	{
		RenderableObject<OGLMesh>* toReturn = nextToDrop;
		nextToDrop = nullptr;
		return toReturn;
	}

	void SetMeshToDrop(OGLMesh* newMesh)
	{
		meshToDrop = newMesh;
	}
};
