#pragma once
#include "OpenGL\OGLMesh.h"
#include "OGLTerrainManager.h"
#include "Collider.h"

class PhysicsObject : public Collider
{
protected:
	RenderableObject<OGLMesh>* object;
	Vector3 m_dir, m_up, m_right;
	float speed;

	float maxSpeed = 1;
	float height = 1;

	OGLTerrainManager* terrainFloor;
	bool grounded;

	bool destroy;
	bool movable;
	
	void ModifyMotionVectorVert(Vector3 modification);
	void SetMotionVectorVert(float yDif);
	void GramSchmidtOrtho(Vector3* changedVec, Vector3* vec1, Vector3* vec2);
	void RotateDirection(float angle);

	Vector3 UpdatePosition(Vector3 currentPos);

	Vector3 CheckFloor(Vector3 foot);
	void UpdateDirByFloor(Vector3 foot);
	void UpdateDirByGravity(Vector3 foot);
		
	void ReactToSphere(PhysicsObject* otherParty);
	void ReactToBox(PhysicsObject* boxObject, PhysicsObject* sphereObject);
	void ReactToBoxBox(PhysicsObject* box1, PhysicsObject* box2);
public:
	PhysicsObject();
	PhysicsObject(OGLTerrainManager* floorTerrain);
	~PhysicsObject();

	virtual void Update();

	void SetMesh(RenderableObject<OGLMesh>* newMesh)
	{
		object = newMesh;
	}

	RenderableObject<OGLMesh>* GetMesh()
	{
		return object;
	}

	void SetMaxSpeed(float newMax)
	{
		maxSpeed = newMax;
	}

	void SetHeight(float newHeight)
	{
		height = newHeight;
	}

	Vector3 GetUp()
	{
		return m_up;
	}

	Vector3 GetDir()
	{
		return m_dir;
	}

	Vector3 GetRight()
	{
		return m_right;
	}

	void SetUp(Vector3 newUp)
	{
		m_up = newUp.Normalise();
	}

	void SetDir(Vector3 newDir)
	{
		m_dir = newDir.Normalise();
	}

	void SetRight(Vector3 newRight)
	{
		m_right = newRight.Normalise();
	}

	void SetSpeed(float newSpeed)
	{
		speed = newSpeed;
	}

	float GetSpeed()
	{
		return speed;
	}

	void SetDestroy(bool newDest)
	{
		destroy = newDest;
	}

	bool DestroyOnCollision()
	{
		return destroy;
	}

	void UpdateByDir()
	{
		GramSchmidtOrtho(&m_dir, &m_up, &m_right);
	}

	virtual Vector3 GetPosition()
	{
		return object->GetLocation();
	}

	virtual void SetPosition(Vector3 newPos)
	{
		object->SetLocation(newPos);
	}

	void MakeStatic()
	{
		movable = false;
	}

	bool CanMove()
	{
		return movable;
	}

	void CheckCollision(PhysicsObject* otherParty)
	{
		//if(parent.collided)
		//parent.reactToCollision()

		if (Collider::Collided(otherParty, GetPosition(), otherParty->GetPosition()))
		{
			ReactToCollision(otherParty);
		}
	}

	bool Collided(PhysicsObject* otherParty)
	{
		return Collider::Collided(otherParty, GetPosition(), otherParty->GetPosition());
	}

	void ReactToCollision(PhysicsObject* otherParty);
};

