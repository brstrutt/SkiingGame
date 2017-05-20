#include "OGLScene.h"
#include "GLEW/include/glew.h"
#include "AIPhysicsObject.h"


OGLScene::OGLScene()
{
}


OGLScene::~OGLScene()
{
}

void OGLScene::CreateCollidable(RenderableObject<OGLMesh>* objectToAdd)
{
	objectToAdd->SetLocation(objectToAdd->GetLocation() + Vector3(0,-5,0));

	AIPhysicsObject *tempPhys = new AIPhysicsObject(floorTer, theCamera->GetCamPos());
	tempPhys->SetMesh(objectToAdd);
	tempPhys->SetMaxSpeed(0.7);
	tempPhys->SetHeight(2.7);
	tempPhys->SetDestroy(true);
	tempPhys->SetCollisionType(box);
	tempPhys->SetDimensions(7, 9, 12);
	physics.push_back(tempPhys);
}

void OGLScene::LoopSats()
{
	OGLMesh *MrSaturn = new OGLMesh(L"../asset/models/MrSaturn.obj", "../asset/texture/SaturnPink.tga");

	for (int i = 0; i < 10; i++)
	{
		RenderableObject<OGLMesh>* temp = new RenderableObject<OGLMesh>();
		temp->SetRenderable(MrSaturn);
		temp->SetLocation(floorTer->GetRandomPoint());

		CreateCollidable(temp);

		theHUD->AddFreeSat();
	}
}

void OGLScene::CreateScene()
{
	theSkybox = new OGLSkybox();
	theSkybox->Init();

	floorTer = new OGLTerrainManager(-1, 50, 10.0);

	OGLMesh *atat = new OGLMesh(L"../asset/models/AT-ATseanbdesigns.obj", "../asset/texture/crate.tga");
	RenderableObject<OGLMesh>* tempatat = new RenderableObject<OGLMesh>();
	tempatat->SetRenderable(atat);
	tempatat->SetLocation(floorTer->GetRandomPoint());

	PhysicsObject* atatCollider1 = new PhysicsObject(floorTer);
	atatCollider1->SetMesh(tempatat);
	atatCollider1->SetDestroy(false);
	atatCollider1->SetCollisionType(box);
	atatCollider1->SetDimensions(265, 500, 1000);
	atatCollider1->SetMaxSpeed(0);
	atatCollider1->MakeStatic();
	physics.push_back(atatCollider1);

	mover = OGLSpaceship();

	OGLMesh *UFO = new OGLMesh(L"../asset/models/UFO.obj", "../asset/texture/tiles_colour.tga");
	RenderableObject<OGLMesh>* temp = new RenderableObject<OGLMesh>();
	temp->SetRenderable(UFO);
	temp->SetLocation(Vector3(10,100,10));
	temp->SetScale(0.1);

	OGLMesh *MrSaturn = new OGLMesh(L"../asset/models/MrSaturn.obj", "../asset/texture/SaturnPink.tga");
	mover = OGLSpaceship(floorTer);
	mover.SetMesh(temp);
	mover.SetMeshToDrop(MrSaturn);
	mover.SetCollisionType(box);
	mover.SetDimensions(45,10,45);
	moverActive = true;

	satPointer = new OGLArrow("../asset/texture/RedLocator.tga");
	satPointer->SetScale(0.5);	

	ufoPointer = new OGLArrow("../asset/texture/BlueLocator.tga");
	ufoPointer->SetScale(0.5);
}

void OGLScene::AddRenderable(Renderable* renderableToAdd)
{
	renderables.push_back(renderableToAdd);
}

void OGLScene::Update()
{
	if (moverActive)
	{
		mover.Update();

		RenderableObject<OGLMesh>* temp = mover.CheckDropState();

		if (temp != NULL)
		{
			CreateCollidable(temp);
			theHUD->AddFreeSat();
		}

		if (thePlayer->Collided(&mover))
		{
			moverActive = false;
			theHUD->DeactiveUFO();

			PhysicsObject *tempPhys = new PhysicsObject(floorTer);
			tempPhys->SetMesh(mover.GetMesh());
			tempPhys->SetCollisionType(box);
			tempPhys->SetDimensions(45, 15, 45);
			physics.push_back(tempPhys);

			renderables.remove(mover.GetMesh()->GetRenderable());

			//mover.GetMesh()->SetLocation(mover.GetMesh()->GetLocation()[0], floorTer->GetHeightAtPoint(mover.GetMesh()->GetLocation()[0], mover.GetMesh()->GetLocation()[2]) - 10, mover.GetMesh()->GetLocation()[2]);
			
			//mover.GetMesh()->SetRotation(1,0,2.14159);
			mover.~OGLSpaceship();
		}
	}

	std::vector<int> indexesToRemove;

	for (int i = 0; i < physics.size(); i++)
	{
		physics.at(i)->Update();
		for (int j = i + 1; j < physics.size(); j++)
		{
			physics.at(i)->CheckCollision(physics.at(j));
		}

		if (thePlayer->Collided(physics.at(i)))
		{
			if (physics.at(i)->DestroyOnCollision())
			{
				indexesToRemove.push_back(i);
				theHUD->GrabSat();
			}
			else
			{
				physics.at(i)->ReactToCollision(thePlayer);
			}

		}

		if (moverActive)
		{
			mover.CheckCollision(physics.at(i));
		}
	}

	for (int i = 0; i < indexesToRemove.size(); i++)
	{
		physics.erase(physics.begin() + indexesToRemove.at(i));
	}
}

void OGLScene::Render()
{
	RendSky();
	RendTerr();

	RendMesh();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	floorTer->RenderSea(glGetUniformLocation(meshShader->GetProgramHandle(), "model"), glGetUniformLocation(meshShader->GetProgramHandle(), "scale"));

	RendBill();

	glDisable(GL_DEPTH_TEST);

	RendHUD();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void OGLScene::RendSky()
{
	skyboxShader->ActivateShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->GetProgramHandle(), "view"), 1, GL_FALSE, theCamera->GetViewMatrix()->GetMatrix());
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->GetProgramHandle(), "projection"), 1, GL_FALSE, theCamera->GetProjMatrix()->GetMatrix());
	glUniformMatrix4fv(glGetUniformLocation(skyboxShader->GetProgramHandle(), "model"), 1, GL_FALSE, theSkybox->GetModelMatrix().GetMatrix());
	glUniform1i(glGetUniformLocation(skyboxShader->GetProgramHandle(), "texColour"), 0);

	theSkybox->Render();
}

void OGLScene::RendMesh()
{
	meshShader->ActivateShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(meshShader->GetProgramHandle(), "view"), 1, GL_FALSE, theCamera->GetViewMatrix()->GetMatrix());
	glUniformMatrix4fv(glGetUniformLocation(meshShader->GetProgramHandle(), "projection"), 1, GL_FALSE, theCamera->GetProjMatrix()->GetMatrix());
	glUniform3fv(glGetUniformLocation(meshShader->GetProgramHandle(), "cameraPosition"), 1, theCamera->GetCameraPosition()->GetData());
	glUniform1i(glGetUniformLocation(meshShader->GetProgramHandle(), "texColour"), 0);

	for each (Renderable* prenderable in renderables)
	{
		glUniformMatrix4fv(glGetUniformLocation(meshShader->GetProgramHandle(), "model"), 1, GL_FALSE, prenderable->GetModelMatrix().GetMatrix());
		glUniform1f(glGetUniformLocation(meshShader->GetProgramHandle(), "scale"), prenderable->GetScale());

		prenderable->Render();
	}

	if (moverActive)
	{
		glUniformMatrix4fv(glGetUniformLocation(meshShader->GetProgramHandle(), "model"), 1, GL_FALSE, mover.GetMesh()->GetModelMatrix().GetMatrix());
		glUniform1f(glGetUniformLocation(meshShader->GetProgramHandle(), "scale"), mover.GetMesh()->GetScale());

		mover.GetMesh()->GetRenderable()->Render();
	}

	for (int i = 0; i < physics.size(); i++)
	{
		glUniformMatrix4fv(glGetUniformLocation(meshShader->GetProgramHandle(), "model"), 1, GL_FALSE, physics.at(i)->GetMesh()->GetModelMatrix().GetMatrix());
		glUniform1f(glGetUniformLocation(meshShader->GetProgramHandle(), "scale"), physics.at(i)->GetMesh()->GetScale());
		physics.at(i)->GetMesh()->GetRenderable()->Render();		
	}
}

void OGLScene::RendTerr()
{
	terrainShader->ActivateShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(terrainShader->GetProgramHandle(), "view"), 1, GL_FALSE, theCamera->GetViewMatrix()->GetMatrix());
	glUniformMatrix4fv(glGetUniformLocation(terrainShader->GetProgramHandle(), "projection"), 1, GL_FALSE, theCamera->GetProjMatrix()->GetMatrix());
	glUniform1f(glGetUniformLocation(terrainShader->GetProgramHandle(), "scale"), 1);
	glUniform1i(glGetUniformLocation(terrainShader->GetProgramHandle(), "texColour1"), 0);
	glUniform1i(glGetUniformLocation(terrainShader->GetProgramHandle(), "texColour2"), 1);

	floorTer->Render(glGetUniformLocation(terrainShader->GetProgramHandle(), "model"));
}

void OGLScene::RendBill()
{
	billboardShader->ActivateShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(billboardShader->GetProgramHandle(), "view"), 1, GL_FALSE, theCamera->GetViewMatrix()->GetMatrix());
	glUniformMatrix4fv(glGetUniformLocation(billboardShader->GetProgramHandle(), "projection"), 1, GL_FALSE, theCamera->GetProjMatrix()->GetMatrix());
	glUniform3fv(glGetUniformLocation(billboardShader->GetProgramHandle(), "cameraPosition"), 1, theCamera->GetCameraPosition()->GetData());
	glUniform1i(glGetUniformLocation(billboardShader->GetProgramHandle(), "texColour"), 0);
	glUniform1i(glGetUniformLocation(billboardShader->GetProgramHandle(), "texColour2"), 1);

	floorTer->RenderBillboards(billboardShader, *(theCamera->GetCameraPosition()));		
}

void OGLScene::RendHUD()
{
	Matrix4x4 tempMat = satPointer->GetModelMatrix();
	for (int i = 0; i < physics.size(); i++)
	{
		if (physics.at(i)->DestroyOnCollision())
		{
			tempMat.SetValue(3, physics.at(i)->GetMesh()->GetLocation()[0]);
			tempMat.SetValue(7, physics.at(i)->GetMesh()->GetLocation()[1] + 15);
			tempMat.SetValue(11, physics.at(i)->GetMesh()->GetLocation()[2]);

			glUniformMatrix4fv(glGetUniformLocation(billboardShader->GetProgramHandle(), "model"), 1, GL_FALSE, tempMat.GetMatrix());
			glUniform1f(glGetUniformLocation(billboardShader->GetProgramHandle(), "scale"), (physics.at(i)->GetMesh()->GetLocation() - *(theCamera->GetCameraPosition())).Norm() * 0.005);
			glUniform3fv(glGetUniformLocation(billboardShader->GetProgramHandle(), "billboardUp"), 1, satPointer->GetRotation().GetData());

			satPointer->Render();
		}
	}

	if (moverActive)
	{
		tempMat = ufoPointer->GetModelMatrix();
		tempMat.SetValue(3, mover.GetMesh()->GetLocation()[0]);
		tempMat.SetValue(7, mover.GetMesh()->GetLocation()[1] + 15);
		tempMat.SetValue(11, mover.GetMesh()->GetLocation()[2]);

		glUniformMatrix4fv(glGetUniformLocation(billboardShader->GetProgramHandle(), "model"), 1, GL_FALSE, tempMat.GetMatrix());
		glUniform1f(glGetUniformLocation(billboardShader->GetProgramHandle(), "scale"), (mover.GetMesh()->GetLocation() - *(theCamera->GetCameraPosition())).Norm() * 0.005);
		glUniform3fv(glGetUniformLocation(billboardShader->GetProgramHandle(), "billboardUp"), 1, ufoPointer->GetRotation().GetData());
		ufoPointer->Render();
	}


	HUDShader->ActivateShaderProgram();

	glUniformMatrix4fv(glGetUniformLocation(HUDShader->GetProgramHandle(), "projection"), 1, GL_FALSE, theCamera->GetProjMatrix()->GetMatrix());

	theHUD->Render(HUDShader);
}