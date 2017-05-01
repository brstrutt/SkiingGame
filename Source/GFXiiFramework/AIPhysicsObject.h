#pragma once
#include "PhysicsObject.h"
class AIPhysicsObject :
	public PhysicsObject
{
private:
	Vector3* playerLocation;
public:
	AIPhysicsObject();
	AIPhysicsObject(OGLTerrainManager* floorTerrain, Vector3* camPos);
	~AIPhysicsObject();

	void Update();


};

