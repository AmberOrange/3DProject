#include "Camera.hpp"

Camera::Camera()
{
	m_position = XMFLOAT3(0.f, 0.f, 0.f);
	m_rotation = XMFLOAT3(0.f, 0.f, 0.f);

	camPosition = XMVectorSet(0.f, 0.f, -5.f, 0.f);

	m_right = XMVectorSet(1.f, 0.f, 0.f,0.f);
	m_up = XMVectorSet(0.f, 1.f, 0.f,0.f);
	m_look = XMVectorSet(0.f, 0.f, 1.f,0.f);
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

bool Camera::Frame(DInput &dinput, double time)
{
	/*XMVECTOR rightVector, lookVector;
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
		);*/


	float speed = 15.0f * time;

	if (dinput.IsKeyPressed(DIK_A))
	{
		moveLeftRight -= speed;
	}
	if (dinput.IsKeyPressed(DIK_D))
	{
		moveLeftRight += speed;
	}
	if (dinput.IsKeyPressed(DIK_W))
	{
		moveBackForward += speed;
	}
	if (dinput.IsKeyPressed(DIK_S))
	{
		moveBackForward -= speed;
	}
	if (dinput.MouseMoved())
	{
		camYaw += dinput.GetMouseState().lX * 0.001f;

		camPitch += dinput.GetMouseState().lY * 0.001f;

		if (camPitch > XM_PIDIV2 - 0.000001f)
			camPitch = XM_PIDIV2 - 0.000001f;
		else if (camPitch < -XM_PIDIV2 + 0.000001f)
			camPitch = -XM_PIDIV2 + 0.000001f;
	}

	return true;
}

// Could use an improvement tbh
void Camera::Render()
{
	//XMVECTOR upVector, positionVector, lookAtVector;
	//XMMATRIX rotationMatrix;

	//positionVector = XMLoadFloat3(&m_position);

	//// Create the rotation matrix from the yaw, pitch, and roll values.
	//rotationMatrix = XMMatrixRotationRollPitchYaw(
	//	m_rotation.x * 0.0174532925f,
	//	m_rotation.y * 0.0174532925f,
	//	m_rotation.z * 0.0174532925f);
	//	

	//// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//lookAtVector = XMVector3TransformCoord(m_look, rotationMatrix);
	//upVector = XMVector3TransformCoord(m_up, rotationMatrix);

	//// Translate the rotated camera position to the location of the viewer.
	//lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	//// Finally create the view matrix from the three updated vectors.
	//m_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);

	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	XMVECTOR camTarget = XMVector3TransformCoord(m_look, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	XMVECTOR camRight = XMVector3TransformCoord(m_right, RotateYTempMatrix);
	XMVECTOR camUp = XMVector3TransformCoord(m_up, RotateYTempMatrix);
	XMVECTOR camForward = XMVector3TransformCoord(m_look, RotateYTempMatrix);

	camPosition += moveLeftRight*camRight;
	camPosition += moveBackForward*camForward;

	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;

	m_viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);

}

void Camera::GetViewMatrix(XMMATRIX &viewMatrix)
{
	viewMatrix = m_viewMatrix;
}
