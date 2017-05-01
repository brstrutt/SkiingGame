#pragma once
#include "OpenGL\OGLMesh.h"
#include "Collider.h"
#include "PhysicsObject.h"

class OGLMeshCollider: public PhysicsObject
{
public:
	OGLMeshCollider();
	OGLMeshCollider(OGLTerrainManager* floorTerrain);
	~OGLMeshCollider();

	void Update();
};

