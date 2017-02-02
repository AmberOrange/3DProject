#pragma once

#include <vector>
#include <sstream>
#include <fstream>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace std;

struct Vertex
{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 Texcoord;
};

class Mesh
{
public:

	Mesh();
	~Mesh();

	virtual bool Initialize(ID3D11Device* device) = 0;

	bool CreateBuffer(ID3D11Device* device);
	bool Draw(ID3D11DeviceContext* deviceContext);

	virtual Mesh* Clone() = 0;
protected:

	ID3D11Buffer* vertexBuffer;

	Vertex* vertices;
	int nrOfVertices;

	bool RHCoordSys;
	bool hasTexcoords;
	bool faceDefAsTriangles;


};