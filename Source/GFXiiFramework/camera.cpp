//Created for Graphics I and II
//Author: Minsi Chen

#include "camera.h"
#include <math.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "quaternion.h"
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>

Camera::Camera()
{
	InitCamera();
}

Camera::~Camera()
{
}

void Camera::InitCamera()
{
	//Initialise the camera that sits at the origin looking down the negative z-axis.
	m_position.SetVector(0.0f, 0.0f, 0.0f);
	m_upVector.SetVector(0.0f, 1.0f, 0.0f);
	m_direction.SetVector(0.0f, 0.0, -1.0f);
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);
	UpdateViewMatrix();
}

void Camera::SetCameraPosition(const Vector3* position)
{
	m_position = (*position);
	//printf("Position: %fX,%fY,%fZ\n", m_position[0], m_position[1], m_position[2]);
	UpdateViewMatrix();
}

void Camera::SetLookAtPoint(const Vector3* lookAt)
{
	m_direction = (*lookAt) - m_position;
	m_direction.Normalise();
	
	//Gram-Schmidt orthogonalisation algorithm
	m_rightVector = m_rightVector - (m_direction * m_rightVector.DotProduct(m_direction));
	m_upVector = m_upVector - (m_direction * m_upVector.DotProduct(m_direction)) - (m_rightVector * (m_upVector.DotProduct(m_rightVector) / m_rightVector.Norm_Sqr()));

	m_rightVector.Normalise();
	m_upVector.Normalise();

	UpdateViewMatrix();
}

void Camera::SetUpVector(const Vector3* up)
{
	m_upVector = (*up);
	
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);

	m_rightVector.Normalise();
	m_upVector.Normalise();

	UpdateViewMatrix();
}

void Camera::SetCameraDirection(Vector3 newDir)
{
	m_direction = newDir;
	m_rightVector = m_rightVector - (m_direction * m_rightVector.DotProduct(m_direction));
	m_upVector = m_upVector - (m_direction * m_upVector.DotProduct(m_direction)) - (m_rightVector * (m_upVector.DotProduct(m_rightVector) / m_rightVector.Norm_Sqr()));

	m_rightVector.Normalise();
	m_upVector.Normalise();

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	float* mat;

	glm::mat4x4 temp = glm::lookAt(
		glm::vec3(m_position[0], m_position[1], m_position[2]), // Camera is at (4,3,3), in World Space
		glm::vec3(m_direction[0] + m_position[0], m_direction[1] + m_position[1], m_direction[2] + m_position[2]), // and looks at..
		glm::vec3(m_upVector[0], m_upVector[1], m_upVector[2]));

	mat = glm::value_ptr(temp);

	m_viewMatrix.SetMatrix(mat);
}

void Camera::SetProjectionMatrix(float* projmat)
{
	m_projectionMatrix.SetMatrix(projmat);
}

void Camera::UpdateProjectionMatrix()
{
	glm::tmat4x4<float> testing = glm::perspective(m_fov, m_aspectRatio, 0.1f, 10000.0f);
	float* temp2;

	temp2 = glm::value_ptr(testing);

	SetProjectionMatrix(temp2);
}

void Camera::StrafeCamera(float amount)
{
	Vector3 newPos = m_position + m_rightVector * amount;
	//newPos[0] += amount;
	SetCameraPosition(&newPos);
}

void Camera::DollyCamera(float amount)
{
	Vector3 newPos = m_position + m_direction * amount;
	//newPos[2] -= amount;
	SetCameraPosition(&newPos);
}
void Camera::PedCamera(float amount)
{
	Vector3 newPos = m_position + m_upVector * amount;
	//newPos[1] += amount;
	SetCameraPosition(&newPos);
}

void Camera::RotateCamera(float yaw, float pitch, float roll)
{
	//Declare rotation quaternions
	Quaternion quatYaw, quatPitch, quatRoll;
	Quaternion quatFinal, quatFinInv;
	Quaternion quatDir, quatUp;
	
	//Rotate the direction
	quatDir.SetQuaternion(m_direction, 0);

	quatPitch.SetQuaternion(m_rightVector * sin(pitch / 2), cos(pitch / 2));
	quatYaw.SetQuaternion(m_upVector * sin(yaw / 2), cos(yaw / 2));
	
	quatFinal = quatPitch * quatYaw;
	quatFinInv.SetQuaternion(quatFinal.GetVector() * -1, quatFinal.GetScalar());

	m_direction = (quatFinal * quatDir * quatFinInv).GetVector();
	m_direction.Normalise();


	//Rotate Up
	quatUp.SetQuaternion(m_upVector, 0);

	quatPitch.SetQuaternion(m_rightVector * sin(pitch / 2), cos(pitch / 2));
	quatRoll.SetQuaternion(m_direction * sin(roll / 2), cos(roll / 2));
	quatYaw.SetQuaternion(m_upVector * sin(yaw / 2), cos(yaw / 2));

	quatFinal = quatPitch * quatRoll * quatYaw;
	quatFinInv.SetQuaternion(quatFinal.GetVector() * -1, quatFinal.GetScalar());

	m_upVector = (quatFinal * quatUp * quatFinInv).GetVector();
	m_upVector.Normalise();


	//Rotate right
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_rightVector.Normalise();

	//printf("Forward:%fX,%fY,%fZ", m_direction[0], m_direction[1], m_direction[2]);
	//printf("Right:%fX,%fY,%fZ\n", m_rightVector[0], m_rightVector[1], m_rightVector[2]);
	//printf("Up:%fX,%fY,%fZ\n", m_upVector[0], m_upVector[1], m_upVector[2]);

	UpdateViewMatrix();
}

void Camera::ZoomCamera(float amount)
{
	float tempFOV = m_fov + amount;
	if ((tempFOV < 8) && (tempFOV > 6.3))
	{
		m_fov = tempFOV;
	}
	UpdateProjectionMatrix();
}
