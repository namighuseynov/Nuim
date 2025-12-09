#pragma once
#include <DirectXMath.h>


class Camera
{
public:
	Camera();

	void SetPerspective(float fovYDegrees, float aspect, float nearZ, float farZ);

	void LookAt(
		const DirectX::XMFLOAT3& eye,
		const DirectX::XMFLOAT3& at,
		const DirectX::XMFLOAT3& up
	);

	DirectX::XMMATRIX GetViewMatrix() const;
	DirectX::XMMATRIX GetProjMatrix() const;

	DirectX::XMFLOAT3 GetPosition() const { return m_position; }

private:
	void UpdateView();
	void UpdateProj();

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_target;
	DirectX::XMFLOAT3 m_up;

	float m_fovY;   // radians
	float m_aspect;
	float m_nearZ;
	float m_farZ;

	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_proj;

};

