#include "OGLTerrain.h"
#include "OpenGL\OGLTexture.h"
#include "GLEW/include/glew.h"
#include <ctime>

OGLTerrain::OGLTerrain()
{
}

OGLTerrain::OGLTerrain(const char* texPath, const char* texPath2)
{
	OGLTexture* meshTex = new OGLTexture();
	meshTex->CreateTextureFromFile(texPath);
	SetTexture(true, meshTex);

	meshTex = new OGLTexture();
	meshTex->CreateTextureFromFile(texPath2);
	SetTexture(false, meshTex);
}


OGLTerrain::~OGLTerrain()
{
}

void OGLTerrain::Render()
{
	unsigned int texHandle1 = dynamic_cast<OGLTexture*>(m_tex1)->m_syshandle;
	unsigned int texHandle2 = dynamic_cast<OGLTexture*>(m_tex2)->m_syshandle;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texHandle2);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glDrawArrays(GL_TRIANGLES, 0, m_numtriangles * 3);

	glBindVertexArray(0);
}

void OGLTerrain::CreateTerrain(int newSquareNum, float newSquareSize, int seed)
{
	srand(seed);

	//num of squares between centrepoint and edge
	squareNum = newSquareNum;
	//size of squares
	squareSize = newSquareSize;
		
	//Generate Y values
	heightPoints = GenerateHeightMap(squareNum);
	//calculate normals
	normals = GenerateNormals(heightPoints, squareNum, squareSize);

	//centrepoint of terrain
	//topLeftPoint = Vector3((squareNum / -2) * squareSize, 0, (squareNum / -2) * squareSize);
	//topLeftPoint = Vector3(0,0,0);
	//number of triangles(assuming a square terrain) is the number of squares times two
	m_numtriangles = (int)pow(squareNum,2) * 2;

	//create the grid of triangles
	Triangle* grid = new Triangle[m_numtriangles];	
	
	int x = 0; int z = 0;
	//loop through grid and initialise the triangles
	for (int i = 0; i < m_numtriangles; i+=2)
	{		
		int tempX, tempZ;
		//define square around (x,z)
		Vector3 pt00, pt01, pt10, pt11;
		Vector3 N00, N01, N10, N11;

		tempX = x; tempZ = z;
		pt00 = Vector3(squareSize * tempX, heightPoints[C2To1(tempX, tempZ, squareNum + 1)], squareSize * tempZ);
		N00 = normals[C2To1(tempX, tempZ, squareNum + 1)];

		tempX = x; tempZ = z + 1;
		pt01 = Vector3(squareSize * tempX, heightPoints[C2To1(tempX, tempZ, squareNum + 1)], squareSize * tempZ);
		N01 = normals[C2To1(tempX, tempZ, squareNum + 1)];

		tempX = x + 1; tempZ = z;
		pt10 = Vector3(squareSize * tempX, heightPoints[C2To1(tempX, tempZ, squareNum + 1)], squareSize * tempZ);
		N10 = normals[C2To1(tempX, tempZ, squareNum + 1)];

		tempX = x + 1; tempZ = z + 1;
		pt11 = Vector3(squareSize * tempX, heightPoints[C2To1(tempX, tempZ, squareNum + 1)], squareSize * tempZ);
		N11 = normals[C2To1(tempX, tempZ, squareNum + 1)];

		grid[i].SetVertices(pt00 , pt01, pt11);
		grid[i].SetNormals(N00, N01, N11);
		grid[i].SetTexCoords(Vector3(1, 0, 0), Vector3(1, 1, 0), Vector3(0, 1, 0));

		grid[i + 1].SetVertices(pt00, pt11, pt10);
		grid[i + 1].SetNormals(N00, N11, N10);
		grid[i + 1].SetTexCoords(Vector3(1, 0, 0), Vector3(0, 1, 0), Vector3(0, 0, 0));

		//update x and z
		x++;
		if (x == squareNum)
		{
			x = 0;
			z++;
		}
	}
	
	int offset = sizeof(Vector3);
	int stride = sizeof(Vertex);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_numtriangles * stride, &(grid[0].m_vertices[0].m_position[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(NULL + offset));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(NULL + 2 * offset));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	delete[] grid;

	theGrass = new Foliage(newSquareNum, newSquareSize, this);
}

float* OGLTerrain::GenerateHeightMap(int squareNum)
{
	//generate list of height values
	int numOfPoints = (int)pow(squareNum + 1, 2);
	float* Ycloud = new float[numOfPoints];

	//int numOfMountains, numOfHills;
	//set all values to null
	for (int i = 0; i < numOfPoints; i += 1)
	{
		if (rand() % 100 > 95)
		{
			Ycloud[i] = (float)max(rand() % 1000, 500);
		}
		else
		{
			Ycloud[i] = (float)max(rand() % 10, 5);
		}
		Ycloud[i + 1] = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		AveragePoints(Ycloud, squareNum);
	}

	return Ycloud;
}

void OGLTerrain::AveragePoints(float* points, int squareNum)
{
	//average edges
	for (int i = 1; i < squareNum; i++)
	{
		points[C2To1(0, i, squareNum + 1)] = Average(
			Average(points[C2To1(0, i - 1, squareNum + 1)], points[C2To1(0, i + 1, squareNum + 1)]),
			Average(points[C2To1(-2, i, squareNum + 1)], points[C2To1(1, i, squareNum + 1)]));

		points[C2To1(squareNum, i, squareNum + 1)] = points[C2To1(0, i, squareNum + 1)];

		points[C2To1(i, 0, squareNum + 1)] = Average(
			Average(points[C2To1(i, -2, squareNum + 1)], points[C2To1(i, 1, squareNum + 1)]),
			Average(points[C2To1(i - 1, 0, squareNum + 1)], points[C2To1(i + 1, 0, squareNum + 1)]));

		points[C2To1(i, squareNum, squareNum + 1)] = points[C2To1(i, 0, squareNum + 1)];
	}

	//average corners
	points[C2To1(0, 0, squareNum + 1)] = Average(
		Average(points[C2To1(0, -2, squareNum + 1)], points[C2To1(0, 1, squareNum + 1)]),
		Average(points[C2To1(-2, 0, squareNum + 1)], points[C2To1(1, 0, squareNum + 1)]));

	points[C2To1(0, squareNum, squareNum + 1)] = points[C2To1(0, 0, squareNum + 1)];
	points[C2To1(squareNum, 0, squareNum + 1)] = points[C2To1(0, 0, squareNum + 1)];

	points[C2To1(squareNum, squareNum, squareNum + 1)] = points[C2To1(0, 0, squareNum + 1)];


	for (int x = 1; x < squareNum; x++)
	{
		for (int z = 1; z < squareNum; z++)
		{
			points[C2To1(x, z, squareNum + 1)] = Average(
				Average(points[C2To1(x, z - 1, squareNum + 1)], points[C2To1(x, z + 1, squareNum + 1)]),
				Average(points[C2To1(x - 1, z, squareNum + 1)], points[C2To1(x + 1, z, squareNum + 1)]));
		}
		points[C2To1(x, 0, squareNum + 1)] = Average(points[C2To1(x, 0, squareNum + 1)], points[C2To1(x, squareNum + 1, squareNum + 1)]);
		points[C2To1(x, squareNum + 1, squareNum + 1)] = points[C2To1(x, 0, squareNum + 1)];
	}

	for (int x = squareNum - 1; x > 0; x--)
	{
		for (int z = squareNum - 1; z > 0; z--)
		{
			points[C2To1(x, z, squareNum + 1)] = Average(
				Average(points[C2To1(x, z - 1, squareNum + 1)], points[C2To1(x, z + 1, squareNum + 1)]),
				Average(points[C2To1(x - 1, z, squareNum + 1)], points[C2To1(x + 1, z, squareNum + 1)]));
		}
		points[C2To1(x, squareNum + 1, squareNum + 1)] = Average(points[C2To1(x, 0, squareNum + 1)], points[C2To1(x, squareNum + 1, squareNum + 1)]);
		points[C2To1(x, 0, squareNum + 1)] = points[C2To1(x, 0, squareNum + 1)];
	}

	//average corners
	points[C2To1(0, 0, squareNum + 1)] = Average(
		Average(points[C2To1(0, -2, squareNum + 1)], points[C2To1(0, 1, squareNum + 1)]),
		Average(points[C2To1(-2, 0, squareNum + 1)], points[C2To1(1, 0, squareNum + 1)]));

	points[C2To1(0, squareNum, squareNum + 1)] = points[C2To1(0, 0, squareNum + 1)];
	points[C2To1(squareNum, 0, squareNum + 1)] = points[C2To1(0, 0, squareNum + 1)];

	points[C2To1(squareNum, squareNum, squareNum + 1)] = points[C2To1(0, 0, squareNum + 1)];

	//average edges
	for (int i = squareNum - 1; i > 0; i--)
	{
		points[C2To1(0, i, squareNum + 1)] = Average(
			Average(points[C2To1(0, i - 1, squareNum + 1)], points[C2To1(0, i + 1, squareNum + 1)]),
			Average(points[C2To1(-2, i, squareNum + 1)], points[C2To1(1, i, squareNum + 1)]));

		points[C2To1(squareNum, i, squareNum + 1)] = points[C2To1(0, i, squareNum + 1)];

		points[C2To1(i, 0, squareNum + 1)] = Average(
			Average(points[C2To1(i, -2, squareNum + 1)], points[C2To1(i, 1, squareNum + 1)]),
			Average(points[C2To1(i - 1, 0, squareNum + 1)], points[C2To1(i + 1, 0, squareNum + 1)]));

		points[C2To1(i, squareNum, squareNum + 1)] = points[C2To1(i, 0, squareNum + 1)];
	}
}

Vector3*  OGLTerrain::GenerateNormals(float* points, int squareNum, float squareSize)
{
	//generate list of normals
	int numOfPoints = (int)pow(squareNum + 1, 2);
	Vector3* normalArr = new Vector3[numOfPoints];

	Vector3 NUL, NUR, NBL, NBR;


	//get normals for edges
	for (int i = 1; i < squareNum; i++)
	{
		NUL = Vector3(0, points[C2To1(0, i + 1, squareNum + 1)], squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(-2, i, squareNum + 1)], 0));
		if (NUL[1] < 0) NUL = NUL * -1;
		NBL = Vector3(0, points[C2To1(0, i - 1, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(-2, i, squareNum + 1)], 0));
		if (NBL[1] < 0) NBL = NBL * -1;

		NUR = Vector3(0, points[C2To1(0, i + 1, squareNum + 1)], squareSize).CrossProduct(Vector3(squareSize, points[C2To1(1, i, squareNum + 1)], 0));
		if (NUR[1] < 0) NUR = NUR * -1;
		NBR = Vector3(0, points[C2To1(0, i - 1, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(squareSize, points[C2To1(1, i, squareNum + 1)], 0));
		if (NBR[1] < 0) NBR = NBR * -1;

		normalArr[C2To1(0, i, squareNum + 1)] = (NUL + NBL + NUR + NBR).Normalise();

		if (normalArr[C2To1(0, i, squareNum + 1)].operator[](1) < 0)
		{
			normalArr[C2To1(0, i, squareNum + 1)] = normalArr[C2To1(0, i, squareNum + 1)] * -1;
		}

		normalArr[C2To1(squareNum, i, squareNum + 1)] = normalArr[C2To1(0, i, squareNum + 1)];

		NUL = Vector3(0, points[C2To1(i, 1, squareNum + 1)], squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(i - 1, 0, squareNum + 1)], 0));
		if (NUL[1] < 0) NUL = NUL * -1;
		NBL = Vector3(0, points[C2To1(i, -2, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(i - 1, 0, squareNum + 1)], 0));
		if (NBL[1] < 0) NBL = NBL * -1;

		NUR = Vector3(0, points[C2To1(i, 1, squareNum + 1)], squareSize).CrossProduct(Vector3(squareSize, points[C2To1(i + 1, 0, squareNum + 1)], 0));
		if (NUR[1] < 0) NUR = NUR * -1;
		NBR = Vector3(0, points[C2To1(i, -2, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(squareSize, points[C2To1(i + 1, 0, squareNum + 1)], 0));
		if (NBR[1] < 0) NBR = NBR * -1;

		normalArr[C2To1(i, 0, squareNum + 1)] = (NUL + NBL + NUR + NBR).Normalise();

		if (normalArr[C2To1(i, 0, squareNum + 1)].operator[](1) < 0)
		{
			normalArr[C2To1(i, 0, squareNum + 1)] = normalArr[C2To1(i, 0, squareNum + 1)] * -1;
		}

		normalArr[C2To1(i, squareNum, squareNum + 1)] = normalArr[C2To1(i, 0, squareNum + 1)];
	}

	//get normals at corners
	NUL = Vector3(0, points[C2To1(0, 1, squareNum + 1)], squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(-2, 0, squareNum + 1)], 0));
	if (NUL[1] < 0) NUL = NUL * -1;
	NBL = Vector3(0, points[C2To1(0, -2, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(-2, 0, squareNum + 1)], 0));
	if (NBL[1] < 0) NBL = NBL * -1;
	NUR = Vector3(0, points[C2To1(0, 1, squareNum + 1)], squareSize).CrossProduct(Vector3(squareSize, points[C2To1(1, 0, squareNum + 1)], 0));
	if (NUR[1] < 0) NUR = NUR * -1;
	NBR = Vector3(0, points[C2To1(0, -2, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(squareSize, points[C2To1(1, 0, squareNum + 1)], 0));
	if (NBR[1] < 0) NBR = NBR * -1;

	normalArr[C2To1(0, 0, squareNum + 1)] = (NUL + NBL + NUR + NBR).Normalise();

	if (normalArr[C2To1(0, 0, squareNum + 1)].operator[](1) < 0)
	{
		normalArr[C2To1(0, 0, squareNum + 1)] = normalArr[C2To1(0, 0, squareNum + 1)] * -1;
	}

	normalArr[C2To1(0, squareNum, squareNum + 1)] = normalArr[C2To1(0, 0, squareNum + 1)];
	normalArr[C2To1(squareNum, 0, squareNum + 1)] = normalArr[C2To1(0, 0, squareNum + 1)];

	normalArr[C2To1(squareNum, squareNum, squareNum + 1)] = normalArr[C2To1(0, 0, squareNum + 1)];

	for (int x = 1; x < squareNum; x++)
	{
		for (int z = 1; z < squareNum; z++)
		{
			//Vector3 normalOne = ((Vector3(0, points[C2To1(x, z - 1, squareNum + 1)], -1 * squareSize) - Vector3(0, points[C2To1(x, z + 1, squareNum + 1)], squareSize)).CrossProduct(
				//Vector3(-1 * squareSize, points[C2To1(x - 1, z, squareNum + 1)], 0) - Vector3(squareSize, points[C2To1(x + 1, z, squareNum + 1)], 0))).Normalise();

			//normalArr[C2To1(x, z, squareNum + 1)] = Vector3(points[C2To1(x - 1, z, squareNum + 1)] - points[C2To1(x + 1, z, squareNum + 1)], points[C2To1(x, z - 1, squareNum + 1)] - points[C2To1(x, z + 1, squareNum + 1)], 2.0).Normalise();
			
			NUL = Vector3(0, points[C2To1(x, z + 1, squareNum + 1)], squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(x - 1, z, squareNum + 1)], 0));
			if (NUL[1] < 0) NUL = NUL * -1;
			NBL = Vector3(0, points[C2To1(x, z - 1, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(-1 * squareSize, points[C2To1(x - 1, z, squareNum + 1)], 0));
			if (NBL[1] < 0) NBL = NBL * -1;

			NUR = Vector3(0, points[C2To1(x, z + 1, squareNum + 1)], squareSize).CrossProduct(Vector3(squareSize, points[C2To1(x + 1, z, squareNum + 1)], 0));
			if (NUR[1] < 0) NUR = NUR * -1;
			NBR = Vector3(0, points[C2To1(x, z - 1, squareNum + 1)], -1 * squareSize).CrossProduct(Vector3(squareSize, points[C2To1(x + 1, z, squareNum + 1)], 0));
			if (NBR[1] < 0) NBR = NBR * -1;

			normalArr[C2To1(x, z, squareNum + 1)] = (NUL + NBL + NUR + NBR).Normalise();

			if (normalArr[C2To1(x, z, squareNum + 1)].operator[](1) < 0)
			{
				normalArr[C2To1(x, z, squareNum + 1)] = normalArr[C2To1(x, z, squareNum + 1)] * -1;
			}
		}
	}
	return normalArr;
}

float OGLTerrain::GetHeightAtPoint(float x, float z)
{
	//position in terrain space
	float terrainSpaceX = (x) / squareSize;
	float terrainSpaceZ = (z) / squareSize;
	
	if (terrainSpaceX < 0 || terrainSpaceZ < 0)
	{
		return 0;
	}
	if (terrainSpaceX > squareNum || terrainSpaceZ > squareNum)
	{
		return 0;
	}

	//grid coords
	int gridX, gridZ;
	gridX = (int)floor(terrainSpaceX);
	gridZ = (int)floor(terrainSpaceZ);

	//depth into square identified by grid coords
	float squareX, squareZ;
	squareX = terrainSpaceX - floor(terrainSpaceX);
	squareZ = terrainSpaceZ - floor(terrainSpaceZ);

	//printf("Worldspace: (%f,%f) ; TerrainSpace: (%d,%d)", x, z, gridX, gridZ);

	float toReturn;

	Vector3 point1, point2, point3;

	//calculate height
	if (squareX >= squareZ)
	{
		//If point is on triangle 00,01,11
		point1 = Vector3(0 , heightPoints[C2To1(gridX, gridZ, squareNum + 1)], 0);
		point2 = Vector3(1, heightPoints[C2To1(gridX + 1, gridZ, squareNum + 1)], 0);
		point3 = Vector3(1, heightPoints[C2To1(gridX + 1, gridZ + 1, squareNum + 1)], 1);
	}
	else
	{
		//If point is on triangle 00,11,10
		point1 = Vector3(0, heightPoints[C2To1(gridX, gridZ, squareNum + 1)], 0);
		point2 = Vector3(1, heightPoints[C2To1(gridX + 1, gridZ + 1, squareNum + 1)], 1);
		point3 = Vector3(0, heightPoints[C2To1(gridX, gridZ + 1, squareNum + 1)], 1);
	}


	toReturn = Barycentric(point1, point2, point3, squareX, squareZ);

	return toReturn; //heightPoints[C2To1(gridX, gridZ, squareNum + 1)];
}

Vector3 OGLTerrain::GetNormalAtPoint(float x, float z)
{
	//position in terrain space
	float terrainSpaceX = (x) / squareSize;
	float terrainSpaceZ = (z) / squareSize;

	if (terrainSpaceX < 0 || terrainSpaceZ < 0)
	{
		return Vector3(0,-1,0);
	}
	if (terrainSpaceX > squareNum || terrainSpaceZ > squareNum)
	{
		return Vector3(0, -1, 0);
	}

	//grid coords
	int gridX, gridZ;
	gridX = (int)floor(terrainSpaceX);// +0.5);
	gridZ = (int)floor(terrainSpaceZ);// +0.5);

	//depth into square identified by grid coords
	float squareX, squareZ;
	squareX = terrainSpaceX - floor(terrainSpaceX);
	squareZ = terrainSpaceZ - floor(terrainSpaceZ);

	Vector3 squarePoint = Vector3(squareX, 0, squareZ);


	Vector3 toReturn;

	Vector3 point1, point2, point3;

	//get normals
	if (squareX >= squareZ)
	{
		//If point is on triangle 00,01,11
		point1 = normals[C2To1(gridX, gridZ, squareNum + 1)];
		//point1 = point1 * (squarePoint - Vector3(0, 0, 0)).Norm();

		point2 = normals[C2To1(gridX + 1, gridZ, squareNum + 1)];
		//point2 = point2 * (squarePoint - Vector3(1, 0, 0)).Norm();

		point3 = normals[C2To1(gridX + 1, gridZ + 1, squareNum + 1)];
		//point3 = point3 * (squarePoint - Vector3(1, 0, 1)).Norm();
	}
	else
	{
		//If point is on triangle 00,11,10
		point1 = normals[C2To1(gridX, gridZ, squareNum + 1)];
		//point1 = point1 * (squarePoint - Vector3(0, 0, 0)).Norm();

		point2 = normals[C2To1(gridX + 1, gridZ + 1, squareNum + 1)];
		//point2 = point2 * (squarePoint - Vector3(1, 0, 1)).Norm();

		point3 = normals[C2To1(gridX, gridZ + 1, squareNum + 1)];
		//point3 = point3 * (squarePoint - Vector3(0, 0, 1)).Norm();
	}

	toReturn = (point1 + point2 + point3).Normalise();

	return toReturn;
}

void OGLTerrain::RenderFoliage(OGLShaderProgram* billShader, Vector3 camPos, Vector3 terrainLocation)
{
	theGrass->RenderFoliage(billShader, camPos, terrainLocation);
}

int OGLTerrain::C2To1(int x, int y, int lineLength)
{
	if (x < 0) x = lineLength + x;
	if (y < 0) y = lineLength + y;

	if (x >= lineLength) x = x - lineLength;
	if (y >= lineLength) y = y - lineLength;

	return x + (y * lineLength);
}

float OGLTerrain::Average(float val1, float val2)
{
	return (val1 + val2) / 2;
}

float OGLTerrain::Barycentric(Vector3 P1, Vector3 P2, Vector3 P3, float givenX, float givenY)
{
	float det = (P2[2] - P3[2])*(P1[0] - P3[0]) + (P3[0] - P2[0])*(P1[2] - P3[2]);
	float l1 = ((P2[2] - P3[2])*(givenX - P3[0]) + (P3[0] - P2[0])*(givenY - P3[2])) / det;
	float l2 = ((P3[2] - P1[2])*(givenX - P3[0]) + (P1[0] - P3[0])*(givenY - P3[2])) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * P1[1] + l2 * P2[1] + l3 * P3[1];
}