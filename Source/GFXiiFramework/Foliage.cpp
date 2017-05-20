#include "Foliage.h"
#include "GLEW/include/glew.h"
#include "OGLTerrain.h"



Foliage::Foliage(int newSquareNum, float newSquareSize, OGLTerrain* terrainPtr) : squareNum(newSquareNum), squareSize(newSquareSize), theTerrain(terrainPtr)
{
	GenerateFoliage();
}


Foliage::~Foliage()
{
}

void Foliage::GenerateFoliage()
{
	int numOfSquares = (int)pow(squareNum, 2);
	int squarea = (int)pow(squareSize, 2);

	//calculate number of pieces of grass
	foliageNumber = numOfSquares * squarea;

	//load grass billboard
	OGLBillboard *tempBoard = new OGLBillboard("../asset/texture/grass.tga");
	tempBoard->CreateBoard();

	//Store the grass billboard
	grassBillboard = new RenderableObject<OGLBillboard>();
	grassBillboard->SetRenderable(tempBoard);
	grassBillboard->SetScale(0.05f);


	int calcedIndex;
	float x, z;

	//Create and store the positions of all pieces of foliage
	foliages = new foliagePoint[numOfSquares * squarea];

	for (int i = 0; i < numOfSquares * squarea; i++)
	{
		foliages[i] = { nullptr,0,0,0,0 };
	}

	int indexX, indexZ;
	int terrainSideLength = squareNum * squareSize;


	for (int i = 0; i < numOfSquares * 3; i++)
	{
		do
		{
			//generate random X/Z coords
			x = (rand() % ((int)(terrainSideLength) + 1));
			z = (rand() % ((int)(terrainSideLength) + 1));

			//Make sure coords are within the terrain area
			if (x < 0) x = terrainSideLength + x;
			if (z < 0) z = terrainSideLength + z;

			if (x >= terrainSideLength) x = x - terrainSideLength;
			if (z >= terrainSideLength) z = z - terrainSideLength;

			//convert coords into an array index(compensating for inability to create variable sized 2d array)
			calcedIndex = x + (z * terrainSideLength);
		} while (foliages[calcedIndex].position != nullptr);

		foliages[calcedIndex] = foliagePoint();
		foliages[calcedIndex].position = new Vector3(x, theTerrain->GetHeightAtPoint(x,z), z);

		//travel downwards telling each point where the current grass is
		//If grass is hit, stop
		for (indexX = x - 1; indexX >= 0; --indexX)
		{
			calcedIndex = indexX + (z * terrainSideLength);

			foliages[calcedIndex].nextUp = x - indexX;

			//exit the loop if the next grass has been found
			if (foliages[calcedIndex].position != nullptr) break;
		}

		//Now do the same upwards
		for (indexX = x + 1; indexX < terrainSideLength; ++indexX)
		{
			calcedIndex = indexX + (z * terrainSideLength);

			foliages[calcedIndex].nextDown = indexX - x;

			//exit the loop if the next grass has been found
			if (foliages[calcedIndex].position != nullptr) break;
		}

		//now repeat horizontally
		for (indexZ = z - 1; indexZ >= 0; --indexZ)
		{
			calcedIndex = x + (indexZ * terrainSideLength);

			foliages[calcedIndex].nextRight = z - indexZ;

			//exit the loop if the next grass has been found
			if (foliages[calcedIndex].position != nullptr) break;
		}

		//Now do the same upwards
		for (indexZ = z + 1; indexZ < terrainSideLength; ++indexZ)
		{
			calcedIndex = x + (indexZ * terrainSideLength);

			foliages[calcedIndex].nextLeft = indexZ - z;

			//exit the loop if the next grass has been found
			if (foliages[calcedIndex].position != nullptr) break;
		}
	}
}

void Foliage::RenderFoliage(OGLShaderProgram* billShader, Vector3 camPos, Vector3 terrainLocation)
{
	int terrainSideLen = squareNum * squareSize;
	
	//Convert the camera position into ints that can be used as indices
	int x = camPos[0] + 0.5f;
	int z = camPos[2] + 0.5f;

	//Move the camera into the area of the terrain square
	while (x >= terrainSideLen)
	{
		x -= terrainSideLen;
	}
	while (z >= terrainSideLen)
	{
		z -= terrainSideLen;
	}

	while (x < 0)
	{
		x += terrainSideLen;
	}
	while (z < 0)
	{
		z += terrainSideLen;
	}	
	int direction = 0; //Set initial Direction to up
	int distance = 2; // Set initial motion to travel 2/2 units
	int convertedDistance;

	//Set the modifications to be applied to the coordinates each loop
	int xDif = 0, zDif = 0;

	int numOfSquares = (int)pow(squareNum, 2);
	int squarea = (int)pow(squareSize, 2);

	int* indexList = new int[numOfSquares * squarea];
	int currentPosition = 0;

	//Repeat X times where X is the number of total rotations required(direction going from 0 to 4)
	for (int i = 0; i < 80; ++i)
	{
		direction = 0;
		for (int j = 0; j < 4; ++j)
		{
			convertedDistance = distance / 2;
			
			for (int k = 0; k < convertedDistance;)
			{
				switch (direction)
				{
				case 0:
					xDif = max(1,min(foliages[x + (z * terrainSideLen)].nextUp,convertedDistance - k));
					zDif = 0;
					break;
				case 1:
					xDif = 0;
					zDif = max(1, min(foliages[x + (z * terrainSideLen)].nextRight, convertedDistance - k));
					break;
				case 2:
					xDif = -1 * max(1, min(foliages[x + (z * terrainSideLen)].nextDown, convertedDistance - k));
					zDif = 0;
					break;
				case 3:
					xDif = 0;
					zDif = -1 * max(1, min(foliages[x + (z * terrainSideLen)].nextLeft, convertedDistance - k));
					break;
				}

				x += xDif;
				z += zDif;

				//Move the camera into the area of the terrain square
				while (x >= terrainSideLen)
				{
					x -= terrainSideLen;
				}
				while (z >= terrainSideLen)
				{
					z -= terrainSideLen;
				}

				while (x < 0)
				{
					x += terrainSideLen;
				}
				while (z < 0)
				{
					z += terrainSideLen;
				}

				
				//Add the foliage at point x,z to the list to be rendered
				indexList[currentPosition] = x + (z * terrainSideLen);

				currentPosition += 1;

				k += abs(xDif + zDif);
			}

			direction += 1;
			distance += 1;

		}

	}


	Matrix4x4 matrixToManipulate;

	int index;

	for (int i = currentPosition; i > 0; --i)
	{
		index = indexList[i - 1];
		if (foliages[index].position != nullptr)
		{

			matrixToManipulate = grassBillboard->GetModelMatrix();

			matrixToManipulate.SetValue(3, foliages[index].position->operator[](0) + terrainLocation[0]);
			matrixToManipulate.SetValue(7, foliages[index].position->operator[](1) + terrainLocation[1]);
			matrixToManipulate.SetValue(11, foliages[index].position->operator[](2) + terrainLocation[2]);

			glUniformMatrix4fv(glGetUniformLocation(billShader->GetProgramHandle(), "model"), 1, GL_FALSE, matrixToManipulate.GetMatrix());
			glUniform1f(glGetUniformLocation(billShader->GetProgramHandle(), "scale"), grassBillboard->GetScale());
			glUniform3fv(glGetUniformLocation(billShader->GetProgramHandle(), "billUp"), 1, grassBillboard->GetRotation().GetData());

			grassBillboard->GetRenderable()->Render();
		}
	}

	delete(indexList);
}
