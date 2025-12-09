#include "NuimDemoPCH.h"
#include "Camera.hpp"

Camera::Camera()
{
	m_position = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);
	m_target = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_up = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);

	m_fovY = DirectX::XMConvertToRadians(60.0f);
	m_aspect = 16.0f / 9.0f;
	m_nearZ = 0.1f;
	m_farZ = 100.0f;

	UpdateView();
	UpdateProj();
}

void Camera::SetPerspective(
	float fovYDegrees, 
	float aspect, 
	float nearZ, 
	float farZ
)
{
	m_fovY = DirectX::XMConvertToRadians(fovYDegrees);
	m_aspect = aspect;
	m_nearZ = nearZ;
	m_farZ = farZ;

	UpdateProj();
}

void Camera::LookAt(
	const DirectX::XMFLOAT3& eye, 
	const DirectX::XMFLOAT3& at, 
	const DirectX::XMFLOAT3& up
)
{
	m_position = eye;
	m_target = at;
	m_up = up;

	UpdateView();
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return XMLoadFloat4x4(&m_view);
}

DirectX::XMMATRIX Camera::GetProjMatrix() const
{
	return XMLoadFloat4x4(&m_proj);
}

void Camera::UpdateView()
{
	DirectX::XMVECTOR eyeVec = DirectX::XMLoadFloat3(&m_position);
	DirectX::XMVECTOR atVec = DirectX::XMLoadFloat3(&m_target);
	DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&m_up);

	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookAtLH(eyeVec, atVec, upVec);
	DirectX::XMStoreFloat4x4(&m_view, viewMat);
}

void Camera::UpdateProj()
{
	DirectX::XMMATRIX projMat = DirectX::XMMatrixPerspectiveFovLH(
		m_fovY,
		m_aspect,
		m_nearZ,
		m_farZ
	);
	DirectX::XMStoreFloat4x4(&m_proj, projMat);
}



