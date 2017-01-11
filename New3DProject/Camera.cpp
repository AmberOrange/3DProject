#include "Camera.hpp"

Camera::Camera()
{
	m_position = XMFLOAT3(0.f, 0.f, 0.f);
	m_rotation = XMFLOAT3(0.f, 0.f, 0.f);

	m_right = XMLoadFloat3(&XMFLOAT3(1.f, 0.f, 0.f));
	m_up = XMLoadFloat3(&XMFLOAT3(0.f, 1.f, 0.f));
	m_look = XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 1.f));
}

Camera::Camera(const Camera &other)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	return;
}

void Camera::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
	return;
}

XMFLOAT3 Camera::GetPosition()
{
	return m_position;
}

XMFLOAT3 Camera::GetRotation()
{
	return m_rotation;
}

bool Camera::Frame(DInput &dinput)
{
	XMVECTOR rightVector, lookVector;
	XMFLOAT3 right, look;
	XMMATRIX rotationMatrix;
	float speed = 0.1f;
	int mouseX, mouseY;

	dinput.GetMouseMovement(mouseX, mouseY);

	m_rotation.y += ((float)mouseX)*speed;
	m_rotation.x += ((float)mouseY)*speed;

	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_rotation.x * 0.0174532925f,
		m_rotation.y * 0.0174532925f,
		m_rotation.z * 0.0174532925f);

	lookVector = XMVector3TransformCoord(m_look, rotationMatrix);
	rightVector = XMVector3TransformCoord(m_right, rotationMatrix);

	XMStoreFloat3(&look, lookVector);
	XMStoreFloat3(&right, rightVector);

	if (dinput.IsKeyPressed(DIK_W))
		m_position = XMFLOAT3(
			m_position.x + look.x * speed,
			m_position.y + look.y * speed,
			m_position.z + look.z * speed
		);
	if (dinput.IsKeyPressed(DIK_S))
		m_position = XMFLOAT3(
			m_position.x - look.x * speed,
			m_position.y - look.y * speed,
			m_position.z - look.z * speed
		);
	if (dinput.IsKeyPressed(DIK_A))
		m_position = XMFLOAT3(
			m_position.x - right.x * speed,
			m_position.y - right.y * speed,
			m_position.z - right.z * speed
		);
	if (dinput.IsKeyPressed(DIK_D))
		m_position = XMFLOAT3(
			m_position.x + right.x * speed,
			m_position.y + right.y * speed,
			m_position.z + right.z * speed
		);

	return true;
}

// Could use an improvement tbh
void Camera::Render()
{
	XMVECTOR upVector, positionVector, lookAtVector;
	XMMATRIX rotationMatrix;

	positionVector = XMLoadFloat3(&m_position);

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(
		m_rotation.x * 0.0174532925f,
		m_rotation.y * 0.0174532925f,
		m_rotation.z * 0.0174532925f);
		

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAtVector = XMVector3TransformCoord(m_look, rotationMatrix);
	upVector = XMVector3TransformCoord(m_up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

}

void Camera::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = m_viewMatrix;
}
