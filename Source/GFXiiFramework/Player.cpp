#include "Player.h"
#include <Windows.h>
#include <math.h>
#include "quaternion.h"

Player::Player()
{
}


Player::Player(Camera* newCam, OGLTerrainManager* floorTerrain) : PhysicsObject(floorTerrain)
{
	theCamera = newCam;
	m_dir = *(theCamera->GetCameraDirection());
	m_up = *(theCamera->GetCameraUpVector());
	m_right = m_dir.CrossProduct(m_up);
	
	camLocked = true;
	lastXState = GetAsyncKeyState('X');
	SetCollisionType(box);
	SetDimension(0.5);
}

Player::~Player()
{
}

void Player::PrintSpeed()
{
	int cutSpeed = (speed * 10 + 0.5f);

	printf("\n\n Speed: [");

	for (int i = 0; i < cutSpeed; i++)
	{
		printf(" || ");
	}

	printf("]\n\n");
}

void Player::Update()
{
	Vector3 foot;

	if (camLocked)
	{
		//get feet pos from eye pos
		Vector3 newCamPos = *(theCamera->GetCameraPosition()) + (m_dir * speed);
		foot = newCamPos;
		foot[1] -= height;

		foot = UpdatePosition(foot);	
		//PrintSpeed();
	}
	else
	{
		//get feet pos from eye pos
		Vector3 newCamPos = *(theCamera->GetCameraPosition());
		foot = newCamPos;
		foot[1] -= height;

		foot = CheckFloor(foot);
	}

	foot[1] += height;
	theCamera->SetCameraPosition(&foot);

	terrainFloor->Update(*(theCamera->GetCameraPosition()));	
}

void Player::GetInput()
{
	if (camLocked)
	{
		float power = 0.001;
		if (grounded)
		{
			power = 0.01;

			if (GetAsyncKeyState(VK_SPACE))
			{
				//Jump
				ModifyMotionVectorVert(Vector3(0, 2, 0));
				printf("JUUUUMP\n");
			}
		}
			
		if (GetAsyncKeyState('W'))
		{
			if (speed < 0.01)
			{
				//printf("YOU'RE TOO SLOW!");
				speed = 0.01;
			}

			ModifyMotionVectorVert(*(theCamera->GetCameraDirection()) * power);
		}
		if (GetAsyncKeyState('A'))
		{
			if (speed < 0.01)
			{
				//printf("YOU'RE TOO SLOW!");
				speed = 0.01;
			}

			ModifyMotionVectorVert(*(theCamera->GetCameraRightVector()) * -power);
		}
		if (GetAsyncKeyState('S'))
		{
			if (speed < 0.01)
			{
				//printf("YOU'RE TOO SLOW!");
				speed = 0.01;
			}

			ModifyMotionVectorVert(*(theCamera->GetCameraDirection()) * -power);
		}
		if (GetAsyncKeyState('D'))
		{
			if (speed < 0.01)
			{
				//printf("YOU'RE TOO SLOW!");
				speed = 0.01;
			}

			ModifyMotionVectorVert(*(theCamera->GetCameraRightVector()) * power);
		}		

		if (GetAsyncKeyState('C'))
		{
			//sync the camera to the player orientation

			theCamera->SetCameraDirection(m_dir);
			theCamera->SetUpVector(new Vector3(m_up));
		}
	}
	else
	{
		if (GetAsyncKeyState('W'))
		{
			theCamera->DollyCamera(1);
		}
		if (GetAsyncKeyState('A'))
		{
			theCamera->StrafeCamera(-1);
		}
		if (GetAsyncKeyState('S'))
		{
			theCamera->DollyCamera(-1);
		}
		if (GetAsyncKeyState('D'))
		{
			theCamera->StrafeCamera(1);
		}
		if (GetAsyncKeyState('R'))
		{
			theCamera->PedCamera(1);
		}
		if (GetAsyncKeyState('F'))
		{
			theCamera->PedCamera(-1);
		}
	}
	if ((GetAsyncKeyState('X')) && (!lastXState))
	{
		camLocked = !camLocked;
		if (camLocked)
		{
			m_dir = *(theCamera->GetCameraDirection());
			m_up = *(theCamera->GetCameraUpVector());
			m_right = m_dir.CrossProduct(m_up);
		}
	}
	lastXState = GetAsyncKeyState('X');
}