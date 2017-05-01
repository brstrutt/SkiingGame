#include "AIPhysicsObject.h"


AIPhysicsObject::AIPhysicsObject()
{
	playerLocation = nullptr;
	grounded = false;
	destroy = true;
}

AIPhysicsObject::AIPhysicsObject(OGLTerrainManager* floorTerrain, Vector3* camPos) : PhysicsObject(floorTerrain)
{
	playerLocation = camPos;
}

AIPhysicsObject::~AIPhysicsObject()
{
}

void AIPhysicsObject::Update()
{
	Vector3 newPos = object->GetLocation() + (m_dir * speed);

	newPos[1] -= height;

	newPos = UpdatePosition(newPos);

	newPos[1] += height;

	object->SetLocation(newPos);

	if (grounded)
	{
		Vector3 plToMe = object->GetLocation() - *playerLocation;

		if (plToMe.Norm() < 200)
		{
			ModifyMotionVectorVert(plToMe.Normalise() * 0.01);
		}
	}	
}