#include "Collider.h"
#include <math.h>

Collider::Collider()
{
	dimensions = nullptr;
}

Collider::Collider(CollisionType collType)
{
	dimensions = nullptr;
	SetCollisionType(collType);
}


Collider::~Collider()
{
}

bool Collider::Collided(Collider* otherCollider, Vector3 thisLocation, Vector3 otherLocation)
{
	if (typeOfCollider == sphere)
	{
		if (otherCollider->GetCollisionType() == sphere)
		{
			return SphereSphereCollided(*dimensions, *otherCollider->GetDimensions(), thisLocation, otherLocation);
		}
		else
		{
			return SphereBoxCollided(*dimensions, otherCollider, thisLocation, otherLocation);
		}
	}
	else
	{
		if (otherCollider->GetCollisionType() == sphere)
		{
			return SphereBoxCollided(*otherCollider->GetDimensions(), this, otherLocation, thisLocation);
		}
		else
		{
			//check box->box collision
			return BoxBoxCollided(this, otherCollider, thisLocation, otherLocation);
		}
	}
}

bool Collider::SphereSphereCollided(float sphereRad1, float sphereRad2, Vector3 sphere1Location, Vector3 sphere2Location)
{
	return (sphere1Location - sphere2Location).Norm() <= (sphereRad1 + sphereRad2);
}

bool Collider::SphereBoxCollided(float sphereRadius, Collider* boxCollider, Vector3 sphereLocation, Vector3 boxLocation)
{
	//exit if bounding spheres don't collide
	if (!SphereSphereCollided(
		sphereRadius,
		sqrtf((boxCollider->GetDimensions()[0] / 2) * (boxCollider->GetDimensions()[0] / 2) + (boxCollider->GetDimensions()[1] / 2) * (boxCollider->GetDimensions()[1] / 2) + (boxCollider->GetDimensions()[2] / 2) * (boxCollider->GetDimensions()[2] / 2)),
		sphereLocation,
		boxLocation))
	{
		return false;
	}

	//exit if:

	//too much X
	if ((sphereLocation[0] - sphereRadius) > boxLocation[0] + (boxCollider->GetDimensions()[0] / 2)) return false;
	//too little X
	if ((sphereLocation[0] + sphereRadius) < boxLocation[0] - (boxCollider->GetDimensions()[0] / 2)) return false;

	//too much Y
	if ((sphereLocation[1] - sphereRadius) > boxLocation[1] + (boxCollider->GetDimensions()[1] / 2)) return false;
	//too little Y
	if ((sphereLocation[1] + sphereRadius) < boxLocation[1] - (boxCollider->GetDimensions()[1] / 2)) return false;

	//too much Z
	if ((sphereLocation[2] - sphereRadius) > boxLocation[2] + (boxCollider->GetDimensions()[2] / 2)) return false;
	//too little Z
	if ((sphereLocation[2] + sphereRadius) < boxLocation[2] - (boxCollider->GetDimensions()[2] / 2)) return false;

	return true;
}

bool Collider::BoxBoxCollided(Collider* box1, Collider* box2, Vector3 box1Loc, Vector3 box2Loc)
{
	//exit if bounding spheres don't collide
	if (!SphereSphereCollided(
		sqrtf((box1->GetDimensions()[0] / 2) * (box1->GetDimensions()[0] / 2) + (box1->GetDimensions()[1] / 2) * (box1->GetDimensions()[1] / 2) + (box1->GetDimensions()[2] / 2) * (box1->GetDimensions()[2] / 2)),
		sqrtf((box2->GetDimensions()[0] / 2) * (box2->GetDimensions()[0] / 2) + (box2->GetDimensions()[1] / 2) * (box2->GetDimensions()[1] / 2) + (box2->GetDimensions()[2] / 2) * (box2->GetDimensions()[2] / 2)),
		box1Loc,
		box2Loc))
	{
		return false;
	}

	//exit if:

	//too much X
	if ((box1Loc[0] - (box1->GetDimensions()[0] / 2)) > box2Loc[0] + (box2->GetDimensions()[0] / 2)) return false;
	//too little X
	if ((box1Loc[0] + (box1->GetDimensions()[0] / 2)) < box2Loc[0] - (box2->GetDimensions()[0] / 2)) return false;

	//too much Y
	if ((box1Loc[1] - (box1->GetDimensions()[1] / 2)) > box2Loc[1] + (box2->GetDimensions()[1] / 2)) return false;
	//too little Y
	if ((box1Loc[1] + (box1->GetDimensions()[1] / 2)) < box2Loc[1] - (box2->GetDimensions()[1] / 2)) return false;

	//too much Z
	if ((box1Loc[2] - (box1->GetDimensions()[2] / 2)) > box2Loc[2] + (box2->GetDimensions()[2] / 2)) return false;
	//too little Z
	if ((box1Loc[2] + (box1->GetDimensions()[2] / 2)) < box2Loc[2] - (box2->GetDimensions()[2] / 2)) return false;

	return true;
}

void Collider::SetCollisionType(CollisionType collType)
{
	typeOfCollider = collType;
	if(dimensions) delete dimensions;
	if (collType == box)
	{
		dimensions = new float[3]{10,10,10};
	}
	else
	{
		dimensions = new float(10);
	}
}
void Collider::SetDimension(float newDimension)
{
	if (typeOfCollider == box)
	{
		dimensions[0] = newDimension;
		dimensions[1] = newDimension;
		dimensions[2] = newDimension;
	}
	else
	{
		*dimensions = newDimension;
	}
}

void Collider::SetDimensions(float newwidth, float newHeight, float newDepth)
{
	if (typeOfCollider == box)
	{
		dimensions[0] = newwidth;
		dimensions[1] = newHeight;
		dimensions[2] = newDepth;
	}
}
