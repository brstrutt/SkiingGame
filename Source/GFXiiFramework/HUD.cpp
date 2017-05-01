#include "HUD.h"
#include "GLEW/include/glew.h"

HUD::HUD()
{
}

HUD::HUD(Player* givenPlayer)
{
	thePlayer = givenPlayer;

	speedUnit = new OGLBillboard("../asset/texture/SpeedBar.tga");
	speedUnit->SetSize(20,2);
	speedUnit->CreateBoard();

	freeSatUnit = new OGLBillboard("../asset/texture/FreeSat.tga");
	freeSatUnit->SetSize(4, 4);
	freeSatUnit->CreateBoard();

	grabSatUnit = new OGLBillboard("../asset/texture/GrabSat.tga");
	grabSatUnit->SetSize(4, 4);
	grabSatUnit->CreateBoard();

	speedTitle = new OGLBillboard("../asset/texture/SpeedBarTitle.tga");
	speedTitle->SetSize(20, 10);
	speedTitle->CreateBoard();

	freeTitle = new OGLBillboard("../asset/texture/FreeSatTitle.tga");
	freeTitle->SetSize(20, 10);
	freeTitle->CreateBoard();

	grabbedTitle = new OGLBillboard("../asset/texture/GrabSatTitle.tga");
	grabbedTitle->SetSize(20, 10);
	grabbedTitle->CreateBoard();

	UFOActiveBill = new OGLBillboard("../asset/texture/activeUFO.tga");
	UFOActiveBill->SetSize(40, 16);
	UFOActiveBill->CreateBoard();

	UFODestroyedBill = new OGLBillboard("../asset/texture/inactiveUFO.tga");
	UFODestroyedBill->SetSize(40, 16);
	UFODestroyedBill->CreateBoard();

	numOfFreeSats = 0;
	numOfGrabbedSats = 0;
	UFOActive = true;
}


HUD::~HUD()
{
}

void HUD::RenderSpeedBar(OGLShaderProgram* theShader)
{
	//render speed bar
	int speed = thePlayer->GetSpeedPercent();
	for (int i = 0; i < speed; i++)
	{
		glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / -3.9f, (i * 2) + (float)*screenY / -13.1f);
		glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), speedUnit->GetScale());
		speedUnit->Render();
	}

	//render title
	glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / -3.9f, ((float)*screenY / -13.1f) - 6);
	glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), speedTitle->GetScale());
	speedTitle->Render();

}

void HUD::RenderFreeSatBar(OGLShaderProgram* theShader)
{
	//render freeSat bar
	for (int i = 0; i < numOfFreeSats; i++)
	{
		glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / 3.9f, (i * 5) + (float)*screenY / -13.0f + 5);
		glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), freeSatUnit->GetScale());
		freeSatUnit->Render();
	}

	//render title
	glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / 3.9f, ((float)*screenY / -13.0f) - 6);
	glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), freeTitle->GetScale());
	freeTitle->Render();
}

void HUD::RenderGrabSatBar(OGLShaderProgram* theShader)
{
	//render grabbedSat bar
	for (int i = 0; i < numOfGrabbedSats; i++)
	{
		glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / 4.4f, (i * 5) + (float)*screenY / -13.0f + 5);
		glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), grabSatUnit->GetScale());
		grabSatUnit->Render();
	}

	//render title
	glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / 4.4f, ((float)*screenY / -13.0f) -6);
	glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), grabbedTitle->GetScale());
	grabbedTitle->Render();
}

void HUD::RenderUFOState(OGLShaderProgram* theShader)
{
	if (UFOActive)
	{
		glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / 4.4f, ((float)*screenY / 15.0f) - 6);
		glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), UFOActiveBill->GetScale());
		UFOActiveBill->Render();
	}
	else
	{
		glUniform2f(glGetUniformLocation(theShader->GetProgramHandle(), "coords"), (float)*screenX / 4.4f, ((float)*screenY / 15.0f) - 6);
		glUniform1f(glGetUniformLocation(theShader->GetProgramHandle(), "scale"), UFODestroyedBill->GetScale());
		UFODestroyedBill->Render();
	}
}

void HUD::Render(OGLShaderProgram* theShader)
{
	RenderSpeedBar(theShader);
	RenderFreeSatBar(theShader);
	RenderGrabSatBar(theShader);
	RenderUFOState(theShader);
}
