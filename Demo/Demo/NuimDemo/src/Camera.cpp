#include "NuimDemoPCH.h"
#include "Camera.hpp"

Camera::Camera()
{
	m_position = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);

	m_yaw = 0.0f;
	m_pitch = 0.0f;

	m_fovY = DirectX::XMConvertToRadians(60.0f);
	m_aspect = 16.0f / 9.0f;
	m_nearZ = 0.1f;
	m_farZ = 100.0f;

	RecalculateDirection();
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

	DirectX::XMVECTOR eyeVec = DirectX::XMLoadFloat3(&eye);
	DirectX::XMVECTOR atVec = DirectX::XMLoadFloat3(&at);
	DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&up);

	DirectX::XMVECTOR forwardVec = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(atVec, eyeVec));

	DirectX::XMStoreFloat3(&m_forward, forwardVec);

	float fx = m_forward.x;
	float fy = m_forward.y;
	float fz = m_forward.z;

	m_pitch = asinf(fy);

	float cosPitch = cosf(m_pitch);
	if (fabsf(cosPitch) > 1e-6f)
	{
		m_yaw = atan2f(fx, fz);
	}
	else
	{
		m_yaw = 0.0f;
	}

	RecalculateDirection();
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

void Camera::SetPosition(const DirectX::XMFLOAT3& pos)
{
	this->m_position = pos;
	UpdateView();
}

void Camera::SetYawPitch(float yawRadians, float pitchRadians)
{
	this->m_yaw = yawRadians;
	this->m_pitch = pitchRadians;

	RecalculateDirection();
	UpdateView();
}

void Camera::AddYawPitch(float deltaYaw, float deltaPitch)
{
	this->m_yaw += deltaYaw;
	this->m_pitch += deltaPitch;

	RecalculateDirection();
	UpdateView();
}

void Camera::MoveLocal(float forward, float right, float up)
{
	DirectX::XMVECTOR forwardVec = DirectX::XMLoadFloat3(&m_forward);
	DirectX::XMVECTOR rightVec = DirectX::XMLoadFloat3(&m_right);
	DirectX::XMVECTOR upVec = DirectX::XMLoadFloat3(&m_up);
	DirectX::XMVECTOR positionVec = DirectX::XMLoadFloat3(&m_position);

	DirectX::XMVECTOR moveVec = DirectX::XMVectorAdd(DirectX::XMVectorScale(forwardVec, forward),
		DirectX::XMVectorScale(rightVec, right));
	moveVec = DirectX::XMVectorAdd(moveVec, DirectX::XMVectorScale(upVec, up));

	positionVec = DirectX::XMVectorAdd(positionVec, moveVec);
	DirectX::XMStoreFloat3(&m_position, positionVec);

	UpdateView();
}

void Camera::UpdateView()
{
	DirectX::XMVECTOR eye = DirectX::XMLoadFloat3(&m_position);
	DirectX::XMVECTOR at = DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&m_position), DirectX::XMLoadFloat3(&m_forward));
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_up);

	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(eye, at, up);
	DirectX::XMStoreFloat4x4(&m_view, view);
}

void Camera::UpdateProj()
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(
		m_fovY,
		m_aspect,
		m_nearZ,
		m_farZ
	);
	DirectX::XMStoreFloat4x4(&m_proj, proj);
}

void Camera::RecalculateDirection()
{
	const float limit = DirectX::XMConvertToRadians(89.0f);
	if (m_pitch > limit)  m_pitch = limit;
	if (m_pitch < -limit) m_pitch = -limit;

	float cosPitch = cosf(m_pitch);
	float sinPitch = sinf(m_pitch);
	float cosYaw = cosf(m_yaw);
	float sinYaw = sinf(m_yaw);

	DirectX::XMFLOAT3 forward;
	forward.x = sinYaw * cosPitch;
	forward.y = sinPitch;
	forward.z = cosYaw * cosPitch;

	m_forward = forward;

	DirectX::XMVECTOR worldUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	DirectX::XMVECTOR f = XMLoadFloat3(&m_forward);

	DirectX::XMVECTOR r = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, f));

	DirectX::XMVECTOR u = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(f, r));

	XMStoreFloat3(&m_right, r);
	XMStoreFloat3(&m_up, u);
	XMStoreFloat3(&m_forward, f);
}



