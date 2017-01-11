#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <DirectXMath.h>
#include "DInput.hpp"

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

	bool Frame(DInput &);
	void Render();
	void GetViewMatrix(XMMATRIX&);

private:
	//float m_positionX, m_positionY, m_positionZ;
	//float m_rotationX, m_rotationY, m_rotationZ;
	XMFLOAT3 m_position, m_rotation;
	XMVECTOR m_up, m_look, m_right;
	XMMATRIX m_viewMatrix;
};

#endif /* CAMERA_HPP */