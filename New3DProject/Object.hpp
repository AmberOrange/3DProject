#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
typedef unsigned int UINT;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 TC;
	XMFLOAT3 Normal;
};

class Object
{
public:

	Object();
	~Object();

	bool createBuffer(ID3D10Device* device);
	bool render(ID3D11DeviceContext* deviceCont);

protected:

	UINT* indices;
	Vertex* vertices;
	ID3D11Buffer* VertBuff;

	int nrOfVertices;
};