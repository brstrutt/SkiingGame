//Created for Graphics I and II
//Author: Minsi Chen

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector3.h"
#include "matrix4x4.h"

class Camera
{
	public:
		enum ECameraType
		{
			CAM_ORTHOGRAPHIC,
			CAM_PERSPECTIVE
		};

	private:
		Matrix4x4						m_viewMatrix;				//View Matrix
		Matrix4x4						m_projectionMatrix;			//Projection Matrix

		Vector3						m_position;					//Position

		Vector3						m_upVector;					//up vector
		Vector3						m_rightVector;				//right vector
		Vector3						m_direction;				//view vector
				
		ECameraType						m_cameratype;				//Projection type

		float							m_fov;						//vertical field of view in degree
		float							m_aspectRatio;		//screen aspect ratio

		float width, height;

	public:
	
										Camera();
		virtual							~Camera();

		void							InitCamera();
		void							SetCameraPosition(const Vector3* position);
		inline const Vector3*			GetCameraPosition() const 
		{
			return &m_position;
		}

		Vector3* GetCamPos()
		{
			return &m_position;
		}

		void							SetLookAtPoint(const Vector3* lookAt);
		inline const Vector3*			GetCameraDirection() const
		{
			return &m_direction;
		}

		void SetCameraDirection(Vector3 newDir);

		void							SetUpVector(const Vector3* up);
		inline const Vector3*			GetCameraUpVector() const
		{
			return &m_upVector;
		}
		inline const Vector3*			GetCameraRightVector() const
		{
			return &m_rightVector;
		}
		inline ECameraType				GetCameraType()
		{
			return m_cameratype;
		}

		inline void						SetCameraFOV(float fov)
		{
			m_fov = fov;
			UpdateProjectionMatrix();
		}

		inline float					GetCameraFOV()
		{
			return m_fov;
		}

		inline void						SetCameraAspectRatio(float ar)
		{
			m_aspectRatio = ar;
			UpdateProjectionMatrix();
		}

		inline float					GetCameraAspectRatio()
		{
			return m_aspectRatio;
		}

		void							UpdateViewMatrix();

		inline const Matrix4x4*			GetViewMatrix() const
		{
			return &m_viewMatrix;
		}

		inline const Matrix4x4*			GetProjMatrix() const
		{
			return &m_projectionMatrix;
		}

		float* GetWidthPointer()
		{
			return &width;
		}

		float* GetHeightPointer()
		{
			return &height;
		}
	
		//TODO: Setup the projection matrix
		void							SetProjectionMatrix(float* projmat);
		void							UpdateProjectionMatrix();
		
		void							StrafeCamera(float amount);
		void							DollyCamera(float amount);
		void							PedCamera(float amount);
		void							RotateCamera(float yaw, float pitch, float roll);
		void							ZoomCamera(float amount);		
		
};

#endif
