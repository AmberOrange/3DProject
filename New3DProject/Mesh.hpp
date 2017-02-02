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
	XMFLOAT2 Texcoord;
	XMFLOAT3 Normal;
};

class Mesh
{
public:

	Mesh();
	~Mesh();

	bool CreateBuffer(ID3D11Device* device);
	bool Draw(ID3D11DeviceContext* deviceContext);

protected:

	ID3D11Buffer* vertexBuffer;

	Vertex* vertices;
	int nrOfVertices;

	bool RHCoordSys;
	bool hasTexcoords;
	bool faceDefAsTriangles;

};