#include "OGLSpaceship.h"
#include "OpenGL\OGLTexture.h"

OGLSpaceship::OGLSpaceship()
{
	GenerateNextPoint(0,0);
	nextToDrop = NULL;
}

OGLSpaceship::OGLSpaceship(OGLTerrainManager* floorTerrain) : PhysicsObject(floorTerrain)
{
	GenerateNextPoint(0, 0);
	nextToDrop = NULL;
}

OGLSpaceship::~OGLSpaceship()
{
}

void OGLSpaceship::GenerateNextPoint(float playX, float playZ)
{
	float x, z;

	x = (rand() % 800) - 400;
	z = (rand() % 800) - 400;

	nextPoint = Vector3(x + playX,100,z + playZ);
}

void OGLSpaceship::Update()
{
	//the spaceship has arrived at the current path point
	if ((nextPoint - object->GetLocation()).Norm() < 5)
	{
		GenerateNextPoint((*playerPosition)[0], (*playerPosition)[2]);

		nextToDrop = new RenderableObject<OGLMesh>();
		nextToDrop->SetLocation(object->GetLocation() + Vector3(0, -5, 0));
		nextToDrop->SetRenderable(meshToDrop);
	}
	
	Vector3 newPos = object->GetLocation() + (m_dir * speed);

	ModifyMotionVectorVert((nextPoint - object->GetLocation()).Normalise() * 0.01);

	newPos = UpdatePosition(newPos);

	newPos[1] = 100;

	object->SetLocation(newPos);
}
