#pragma once
#include "Player.h"
class HUD
{
private:
	Player *thePlayer;
	int numOfFreeSats, numOfGrabbedSats;

	OGLBillboard *speedUnit, *freeSatUnit, *grabSatUnit;
	OGLBillboard *speedTitle, *freeTitle, *grabbedTitle;
	OGLBillboard *UFOActiveBill, *UFODestroyedBill;

	int *screenX, *screenY;
	bool UFOActive;

	void RenderSpeedBar(OGLShaderProgram* theShader);
	void RenderFreeSatBar(OGLShaderProgram* theShader);
	void RenderGrabSatBar(OGLShaderProgram* theShader);
	void RenderUFOState(OGLShaderProgram* theShader);

public:
	HUD();
	HUD(Player* givenPlayer);
	~HUD();
	
	void Render(OGLShaderProgram* theShader);

	void SetScreenSize(int* newX, int* newY)
	{
		screenX = newX;
		screenY = newY;
	}

	void AddFreeSat()
	{
		numOfFreeSats++;
	}
	void GrabSat()
	{
		numOfFreeSats--;
		numOfGrabbedSats++;
	}

	void DeactiveUFO()
	{
		UFOActive = false;
	}
};
