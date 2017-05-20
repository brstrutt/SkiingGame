#pragma once
#include "camera.h"
#include "Vector3.h"
#include "OGLTerrainManager.h"
#include "PhysicsObject.h"

class Player : public PhysicsObject
{
private:
	Camera* theCamera;
	bool camLocked, lastXState;

	void PrintSpeed();
public:
	Player();
	Player(Camera* newCam, OGLTerrainManager* floorTerrain);
	~Player();

	void Update();
	
	void GetInput();

	float GetSpeedPercent()
	{
		return (speed / maxSpeed) * 100;
	}

	Vector3 GetPosition()
	{
		return *theCamera->GetCameraPosition() + Vector3(0,-height/2,0);
	}

	void SetPosition(Vector3 newPos)
	{
		theCamera->SetCameraPosition(new const Vector3(newPos[0], newPos[1] + height / 2, newPos[2]));
	}
};

