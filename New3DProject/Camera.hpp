#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <DirectXMath.h>
#include "Input.hpp"

using namespace DirectX;

class Camera
{
public:
	Camera();
	Camera(const Camera &);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

<<<<<<< HEAD
	bool Frame(Input &);
=======
	bool Frame(DInput &dinput, double time);
>>>>>>> bdd00f26f199416230860948fb6269d14c9ea122
	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	//float m_positionX, m_positionY, m_positionZ;
	//float m_rotationX, m_rotationY, m_rotationZ;

	XMFLOAT3 m_position, m_rotation;
	XMVECTOR m_up, m_look, m_right;
	XMVECTOR camPosition;
	XMMATRIX m_viewMatrix;
	float moveLeftRight = 0.0f;
	float moveBackForward = 0.0f;

	float camYaw = 0.0f;
	float camPitch = 0.0f;
};

#endif /* CAMERA_HPP */