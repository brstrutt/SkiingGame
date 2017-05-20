#pragma once
#include "Vector3.h"

enum CollisionType
{
	sphere = 0,
	box,
};

class Collider
{
protected:	
	CollisionType typeOfCollider;

	float* dimensions;

	bool Collided(Collider* otherCollider, Vector3 thisLocation, Vector3 otherLocation);

	bool SphereSphereCollided(float sphereRad1, float sphereRad2, Vector3 sphere1Location, Vector3 sphere2Location);
	bool SphereBoxCollided(float sphereRadius, Collider* boxCollider, Vector3 sphereLocation, Vector3 boxLocation);
	bool BoxBoxCollided(Collider* box1, Collider* box2, Vector3 box1Loc, Vector3 box2Loc);

public:
	Collider();
	Collider(CollisionType collType);
	~Collider();

	void SetCollisionType(CollisionType collType);
	void SetDimension(float newDimension);
	void SetDimensions(float newwidth, float newHeight, float newDepth);
	CollisionType GetCollisionType()
	{
		return typeOfCollider;
	}
	float* GetDimensions()
	{
		return dimensions;
	}
};
