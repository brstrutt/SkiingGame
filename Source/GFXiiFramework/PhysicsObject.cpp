#include "PhysicsObject.h"
#include "quaternion.h"

PhysicsObject::PhysicsObject()
{
}

PhysicsObject::PhysicsObject(OGLTerrainManager* floorTerrain) : Collider(sphere)
{
	speed = 0.5;
	terrainFloor = floorTerrain;

	m_dir = Vector3(1, 0, 0);
	m_up = Vector3(0, 1, 0);
	m_right = Vector3(0, 0, 1);

	grounded = true;
	destroy = false;
	movable = true;
}


PhysicsObject::~PhysicsObject()
{
}

void PhysicsObject::ModifyMotionVectorVert(Vector3 modification)
{
	m_dir = m_dir * speed;
	m_dir = m_dir + modification;

	speed = abs(m_dir.Norm());
	m_dir.Normalise();

	GramSchmidtOrtho(&m_dir, &m_up, &m_right);
}

void PhysicsObject::SetMotionVectorVert(float yDif)
{
	m_dir = m_dir * speed;
	m_dir[1] = yDif;

	speed = abs(m_dir.Norm());
	m_dir.Normalise();

	GramSchmidtOrtho(&m_dir, &m_up, &m_right);
}

void PhysicsObject::GramSchmidtOrtho(Vector3* changedVec, Vector3* vec1, Vector3* vec2)
{
	*vec1 = *vec1 - (*changedVec * vec1->DotProduct(*changedVec));
	*vec2 = *vec2 - (*changedVec * vec2->DotProduct(*changedVec)) - (*vec1 * (vec2->DotProduct(*vec1) / vec1->Norm_Sqr()));

	vec1->Normalise();
	vec2->Normalise();
}

void PhysicsObject::RotateDirection(float angle)
{
	Quaternion rotation, invRotation, motionQuat, rightQuat;

	Vector3 toRotateAround;

	if (grounded)
	{
		toRotateAround = m_up;
	}
	else
	{
		toRotateAround = Vector3(0, 1, 0);
	}

	motionQuat.SetQuaternion(m_dir, 0);
	rightQuat.SetQuaternion(m_right, 0);

	rotation.SetQuaternion(toRotateAround * sin(angle / 2), cos(angle / 2));
	invRotation.SetQuaternion(rotation.GetVector() * -1, rotation.GetScalar());

	m_dir = (rotation * motionQuat * invRotation).GetVector();
	m_right = (rotation * rightQuat * invRotation).GetVector();


	//ModifyMotionVectorVert(Vector3(angle, 0, 0));
}

Vector3 PhysicsObject::UpdatePosition(Vector3 currentPos)
{
	currentPos = CheckFloor(currentPos);

	Vector3 oldDir = m_dir;

	UpdateDirByFloor(currentPos);
	
	if ((currentPos[1] > 20)&&grounded)
	{
		speed = max(speed - 0.002, 0);
	}

	UpdateDirByGravity(currentPos);

	oldDir = oldDir - m_dir;

	//limit speed
	if (speed > maxSpeed) speed = maxSpeed;
	
	return currentPos;
}

Vector3 PhysicsObject::CheckFloor(Vector3 foot)
{
	float floorHeight = terrainFloor->GetHeightAtPoint(foot[0], foot[2]);

	//place feet on floor
	if (foot[1] <= floorHeight + 0.2)
	{
		foot[1] = floorHeight;
		grounded = true;
	}
	else
	{
		grounded = false;
	}

	return foot;
}

void PhysicsObject::UpdateDirByFloor(Vector3 foot)
{
	float floorHeight = terrainFloor->GetHeightAtPoint(foot[0], foot[2]);

	if (grounded)
	{
		Vector3 newUp = Vector3(terrainFloor->GetNormalAtPoint(foot[0], foot[2]));
		m_up = newUp;


		//rotate and generally modify shiiet
		Vector3 tempDir = m_dir;

		//rotate tempDir till m_up is vector 0,1,0
		Quaternion rotation, invRotation, toRot;

		rotation.SetQuatByVectors(newUp, Vector3(0, 1, 0));
		invRotation.SetQuaternion(rotation.GetVector() * -1, rotation.GetScalar());

		toRot.SetQuaternion(tempDir, 0);

		tempDir = (rotation * toRot * invRotation).GetVector();

		//remove the y component
		tempDir[1] = 0;

		//rotate back
		rotation.SetQuaternion(rotation.GetVector(), rotation.GetScalar() * -1);
		invRotation.SetQuaternion(rotation.GetVector() * -1, rotation.GetScalar());

		toRot.SetQuaternion(tempDir, 0);

		tempDir = (rotation * toRot * invRotation).GetVector();
		//set dir to temp dir

		m_dir = tempDir;

		speed *= abs(m_dir.Norm());
		m_dir.Normalise();

		m_right = m_dir.CrossProduct(m_up);
	}
}

void PhysicsObject::UpdateDirByGravity(Vector3 foot)
{
	float floorHeight = terrainFloor->GetHeightAtPoint(foot[0], foot[2]);
	Vector3 gravity = Vector3(0, -0.00981, 0);

	if (grounded)
	{
		//apply gravity force based on slope!!

		Vector3 normal = Vector3(terrainFloor->GetNormalAtPoint(foot[0], foot[2]));

		//rotate and generally modify shiiet
		Vector3 resultForce = gravity;

		//rotate tempDir till m_up is vector 0,1,0
		Quaternion rotation, invRotation, toRot;

		rotation.SetQuatByVectors(normal, Vector3(0, 1, 0));
		invRotation.SetQuaternion(rotation.GetVector() * -1, rotation.GetScalar());

		toRot.SetQuaternion(resultForce, 0);

		resultForce = (rotation * toRot * invRotation).GetVector();

		//remove the y component
		resultForce[1] = 0;

		//rotate back
		rotation.SetQuaternion(rotation.GetVector(), rotation.GetScalar() * -1);
		invRotation.SetQuaternion(rotation.GetVector() * -1, rotation.GetScalar());

		toRot.SetQuaternion(resultForce, 0);

		resultForce = (rotation * toRot * invRotation).GetVector();
		//set dir to temp dir

		ModifyMotionVectorVert(resultForce);

		GramSchmidtOrtho(&m_dir, &m_up, &m_right);
	}
	else
	{
		ModifyMotionVectorVert(gravity);
	}
}

void PhysicsObject::Update()
{
	Vector3 newPos = object->GetLocation() + (m_dir * speed);

	newPos[1] -= height;

	newPos = UpdatePosition(newPos);

	newPos[1] += height;

	object->SetLocation(newPos);
}

void PhysicsObject::ReactToCollision(PhysicsObject* otherParty)
{
	if (typeOfCollider == sphere)
	{
		if (otherParty->GetCollisionType() == sphere)
		{
			ReactToSphere(otherParty);
		}
		else
		{
			ReactToBox(otherParty, this);
		}
	}
	else
	{
		if (otherParty->GetCollisionType() == sphere)
		{
			ReactToBox(this, otherParty);
		}
		else
		{
			if (otherParty->CanMove())
			{				
				ReactToBoxBox(this, otherParty);
			}
			else
			{
				ReactToBoxBox(otherParty, this);
			}			
		}
	}
}

void PhysicsObject::ReactToSphere(PhysicsObject* otherParty)
{
	if (grounded)
	{
		speed *= -4;
		otherParty->SetSpeed(otherParty->GetSpeed() * -4);
		Update();
		otherParty->Update();
		speed *= -0.25;
		otherParty->SetSpeed(otherParty->GetSpeed() * -0.25);
	}

	Vector3 tempUp, tempDir, tempRight;
	float tempSpeed;

	tempUp = m_up;
	tempDir = m_dir;
	tempRight = m_right;

	tempSpeed = speed;

	m_up = otherParty->GetUp();
	m_dir = otherParty->GetDir();
	m_right = otherParty->GetRight();

	otherParty->SetUp(tempUp);
	otherParty->SetDir(tempDir);
	otherParty->SetRight(tempRight);

	speed = otherParty->GetSpeed();
	otherParty->SetSpeed(tempSpeed);
}

void PhysicsObject::ReactToBox(PhysicsObject* boxObject, PhysicsObject* sphereObject)
{
	int smallestDif = 3;
	float lastDif = 0;
	
	Vector3 rayFromSphereToBox = boxObject->GetPosition() - sphereObject->GetPosition();

	for (int i = 0; i < 3; i++)
	{
		if (abs(rayFromSphereToBox[i] / boxObject->GetDimensions()[i]) > lastDif)
		{
			smallestDif = i;
			lastDif = abs(rayFromSphereToBox[i]/boxObject->GetDimensions()[i]);
		}
	}

	//modify the sphere's position to no longer be colliding

	if (smallestDif != 3)
	{
		Vector3 tempSpherPos = sphereObject->GetPosition();

		if (rayFromSphereToBox[smallestDif] > 0 && smallestDif != 1)
		{
			tempSpherPos[smallestDif] = (boxObject->GetPosition()[smallestDif] - boxObject->GetDimensions()[smallestDif] / 2) - (*sphereObject->GetDimensions());// +0.01);
		}
		else
		{
			tempSpherPos[smallestDif] = (boxObject->GetPosition()[smallestDif] + boxObject->GetDimensions()[smallestDif] / 2) + (*sphereObject->GetDimensions());// +0.01);
		}

		sphereObject->SetPosition(tempSpherPos);

		Vector3 tempSphereDir = sphereObject->GetDir();

		tempSphereDir[smallestDif] *= -0.5;

		sphereObject->SetDir(tempSphereDir);
		sphereObject->UpdateByDir();

		Vector3 tempBoxDir = boxObject->GetDir();

		tempBoxDir[smallestDif] -= tempSphereDir[smallestDif];
		boxObject->SetDir(tempBoxDir);
		boxObject->UpdateByDir();
	}
}

void PhysicsObject::ReactToBoxBox(PhysicsObject* box1, PhysicsObject* box2)
{
	int smallestDif = 3;
	float lastDif = 0;

	Vector3 moddedBoxPos1 = box1->GetPosition(), moddedBoxPos2 = box2->GetPosition();

	for (int i = 0; i < 3; i++)
	{
		if (moddedBoxPos1[i] > moddedBoxPos2[i])
		{
			moddedBoxPos1[i] += (box1->GetDimensions()[i] + box2->GetDimensions()[i]) / 2;
		}
		else
		{
			moddedBoxPos1[i] -= (box1->GetDimensions()[i] + box2->GetDimensions()[i]) / 2;
		}
	}

	Vector3 rayFromSphereToBox = moddedBoxPos1 - moddedBoxPos2;

	for (int i = 0; i < 3; i++)
	{
		if (abs(rayFromSphereToBox[i] / (box1->GetDimensions()[i] + box2->GetDimensions()[i])) > lastDif)
		{
			smallestDif = i;
			lastDif = abs(rayFromSphereToBox[i] / (box1->GetDimensions()[i] + box2->GetDimensions()[i]));
		}
	}

	//modify the sphere's position to no longer be colliding

	if (smallestDif != 3)
	{
		Vector3 tempSpherPos = box2->GetPosition();

		if (rayFromSphereToBox[smallestDif] > 0 && smallestDif != 1)
		{
			tempSpherPos[smallestDif] = (box1->GetPosition()[smallestDif] - box1->GetDimensions()[smallestDif] / 2) - (box2->GetDimensions()[smallestDif]/2);
		}
		else
		{
			tempSpherPos[smallestDif] = (box1->GetPosition()[smallestDif] + box1->GetDimensions()[smallestDif] / 2) + (box2->GetDimensions()[smallestDif] / 2);
		}

		box2->SetPosition(tempSpherPos);

		Vector3 tempSphereDir = box2->GetDir();

		tempSphereDir[smallestDif] *= -0.5;

		box2->SetDir(tempSphereDir);
		box2->UpdateByDir();

		Vector3 tempBoxDir = box1->GetDir();

		tempBoxDir[smallestDif] -= tempSphereDir[smallestDif];
		box1->SetDir(tempBoxDir);
		box1->UpdateByDir();
	}
}