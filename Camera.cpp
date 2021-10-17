#include "Camera.h"
using namespace Elite;
Camera* Camera::m_pInstance{ nullptr };

Camera* Camera::getInstance(uint32_t width, uint32_t height,Elite::FPoint3 pos,Elite::FVector3 dir)
{
	if (!m_pInstance)
	{
		m_pInstance = new Camera(width,height,pos,dir);
	}
	return m_pInstance;
}

Camera::Camera(uint32_t width, uint32_t height, Elite::FPoint3 pos, Elite::FVector3 dir)
	:m_WorldUpVector{ 0,1,0 }
	, m_Position{ pos }
	, m_ForwardVector{ dir }
	, m_AngleOfFOV{ 45.0f }
	, m_FOV{ tanf(Elite::ToRadians(m_AngleOfFOV / 2.f)) }

{
	m_UpVector={ 0.f, 1.f, 0.f }; 
	m_RightVector={ 1.f, 0.f, 0.f }; 

	m_AspectRatio = float(width) / height;

}

float Camera::GetFOV() const
{
    return m_FOV;
}

Elite::FMatrix4 Camera::GetLookAtMatrix()
{
    Normalize(m_ForwardVector);
    m_RightVector = Cross(m_WorldUpVector, m_ForwardVector);
    Normalize(m_RightVector);
    m_UpVector = Cross(m_ForwardVector, m_RightVector);
    Normalize(m_UpVector);

	return { m_RightVector.x, m_UpVector.x, m_ForwardVector.x,m_Position.x
			 ,m_RightVector.y, m_UpVector.y, m_ForwardVector.y,m_Position.y
			 ,m_RightVector.z, m_UpVector.z, m_ForwardVector.z,m_Position.z
			 ,0			  ,		0	  ,		0		   ,		1 };
}

void Camera::TranslateX(float distanceX)
{
	m_Position += m_RightVector * distanceX;
}
void Camera::TranslateY(float distanceY)
{
	m_Position += m_UpVector * distanceY;
}
void Camera::TranslateZ(float distanceZ)
{
	m_Position += m_ForwardVector * distanceZ;
}

void Camera::Pitch(float angle)
{
    m_ForwardVector = Inverse(Transpose(MakeRotation(Elite::ToRadians(angle), m_RightVector))) * m_ForwardVector;
}

void Camera::Yaw(float angle)
{
	m_ForwardVector = Inverse(Transpose(MakeRotation(Elite::ToRadians(angle), m_UpVector))) * m_ForwardVector;
}

Elite::FPoint3 Camera::GetPosition() const
{
    return m_Position;
}
Elite::FVector3 Camera::GetForwardVector() const
{
	return m_ForwardVector;
}

float Camera::GetAspectRatio() const
{
	return m_AspectRatio;
}
