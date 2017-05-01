#include "OGLTerrainManager.h"
#include "GLEW/include/glew.h"
#include "OpenGL\OGLTexture.h"
#include <ctime>

OGLTerrainManager::OGLTerrainManager()
{	
}

OGLTerrainManager::OGLTerrainManager(int new_seed, int sq_num, float sq_size)
{
	if (new_seed != -1)
	{
		seed = new_seed;
	}
	else
	{
		seed = time(0);
	}
	
	squareNum = sq_num;
	squareSize = sq_size;
	seaLevel = 20;

	GenerateTerrain();

	FocusOnPoint(Vector3(0, 0, 0));

	/*OGLTexture* temp = new OGLTexture();

	temp->CreateTextureFromFile("../asset/texture/need.tga");

	terr[4].SetTexture(temp);*/

	srand(time(0));
}


OGLTerrainManager::~OGLTerrainManager()
{

}

void OGLTerrainManager::GenerateTerrain()
{
	//create the terrains
	terr = new RenderableObject<OGLTerrain>[4];

	OGLTerrain* tempTerr = new OGLTerrain("../asset/texture/snow.tga","../asset/texture/dirtySnow2.tga");
	tempTerr->CreateTerrain(squareNum, squareSize, seed);

	for (int i = 0; i < 4; i++)
	{
		terr[i].SetRenderable(tempTerr);
	}

	theSea = new OGLBillboard("../asset/texture/Ice.tga");
	theSea->SetUVs(squareNum * squareSize/5, squareNum * squareSize/5);
	theSea->CreateBoard();
	theSea->SetScale(squareNum * squareSize/5);
	theSea->SetRotation(-3.14159 / 2, 0, 0);
}


void OGLTerrainManager::FocusOnPoint(Vector3 newCentre)
{
	float size = squareSize * squareNum;
	terr[0].SetLocation(newCentre + Vector3(-size, 0, -size));
	terr[1].SetLocation(newCentre + Vector3(0, 0, -size));
	terr[2].SetLocation(newCentre + Vector3(-size, 0, 0));
	terr[3].SetLocation(newCentre + Vector3(0, 0, 0));

	centreSquareCoords = newCentre;

	theSea->SetLocation(newCentre[0], seaLevel, newCentre[2] + size);
}

void OGLTerrainManager::Update(Vector3 playerPos)
{
	Vector3 relativePos = playerPos - centreSquareCoords;
	float sizeByTwo = (squareNum * squareSize) / 2.0f;

	if (relativePos[0] > sizeByTwo)
	{
		FocusOnPoint(centreSquareCoords + Vector3(sizeByTwo * 2, 0, 0));
	}

	if (relativePos[0] < -1 * sizeByTwo)
	{
		FocusOnPoint(centreSquareCoords + Vector3(sizeByTwo * -2, 0, 0));
	}

	if (relativePos[2] > sizeByTwo)
	{
		FocusOnPoint(centreSquareCoords + Vector3(0, 0, sizeByTwo * 2));
	}

	if (relativePos[2] < -1 * sizeByTwo)
	{
		FocusOnPoint(centreSquareCoords + Vector3(0, 0, sizeByTwo * -2));
	}
}

void OGLTerrainManager::Render(int m_uniform_model)
{
	for (int i = 0; i < 4; i++)
	{
		glUniformMatrix4fv(m_uniform_model, 1, GL_FALSE, terr[i].GetModelMatrix().GetMatrix());
		terr[i].GetRenderable()->Render();
	}
}

void OGLTerrainManager::RenderSea(int m_uniform_model, int m_uniform_scale)
{
	glUniformMatrix4fv(m_uniform_model, 1, GL_FALSE, theSea->GetModelMatrix().GetMatrix());
	glUniform1f(m_uniform_scale, theSea->GetScale());
	theSea->Render();
}

void OGLTerrainManager::RenderBillboards(OGLShaderProgram* billShader, Vector3 camPos)
{	
	for (int i = 0; i < 4; i++)
	{
		terr[i].GetRenderable()->RenderFoliage(billShader, camPos, terr[i].GetLocation());
	}
}

float OGLTerrainManager::GetHeightAtPoint(float x, float z)
{
	//Vector3 quadrantCentre = GetQuadrantCentre(Vector3(x, 0, z));

	float size = squareNum * squareSize;

	while (x < 0) x += size;
	while (z < 0) z += size;

	while (x > size) x -= size;
	while (z > size) z -= size;

	return max(terr[3].GetRenderable()->GetHeightAtPoint(x, z), seaLevel - 0.1);// -quadrantCentre[0], z - quadrantCentre[2]);
}

Vector3 OGLTerrainManager::GetNormalAtPoint(float x, float z)
{
	//Vector3 quadrantCentre = GetQuadrantCentre(Vector3(x, 0, z));

	float size = squareNum * squareSize;

	while (x < 0) x += size;
	while (z < 0) z += size;

	while (x > size) x -= size;
	while (z > size) z -= size;

	if (terr[3].GetRenderable()->GetHeightAtPoint(x, z) < seaLevel)
	{
		return Vector3(0,1,0);
	}
	else
	{
		return terr[3].GetRenderable()->GetNormalAtPoint(x, z);
	}
}

Vector3 OGLTerrainManager::GetRandomPoint()
{
	float x, z;

	x = (rand() % ((int)(squareNum * squareSize) + 1));
	z = (rand() % ((int)(squareNum * squareSize) + 1));

	Vector3 toReturn = Vector3(x, terr[3].GetRenderable()->GetHeightAtPoint(x,z), z);

	//printf("GennedLoc: x, %f; y, %f; z, %f;\n", toReturn[0], toReturn[1], toReturn[2]);

	return toReturn + centreSquareCoords;
}