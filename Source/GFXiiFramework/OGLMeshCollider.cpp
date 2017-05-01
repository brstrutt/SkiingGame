#include "OGLMeshCollider.h"


OGLMeshCollider::OGLMeshCollider()
{
}

OGLMeshCollider::OGLMeshCollider(OGLTerrainManager* floorTerrain) : PhysicsObject(floorTerrain)
{
}


OGLMeshCollider::~OGLMeshCollider()
{
}

void OGLMeshCollider::Update()
{
	speed = 0;
}
