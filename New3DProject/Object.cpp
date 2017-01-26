#include "Object.hpp"

bool Object::loadObj(wstring filename)
{
	HRESULT hr;
	ifstream fileIn;

	// Temporary vectors to be used in vertices.
	vector<XMFLOAT3> vPos;
	vector<XMFLOAT2> vTC;
	vector<XMFLOAT3> vNor;

	vector<UINT> vPosIndex;
	vector<UINT> vTCIndex;
	vector<UINT> vNorIndex;
}

bool Object::createBuffer(ID3D11Device* device)
{
	D3D11_BUFFER_DESC vertexBuffDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT hr;

	// Clear memory from unwanted data.
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
	ZeroMemory(&vertexData, sizeof(vertexData));

	// Set up description of the vertex buffer.
	vertexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBuffDesc.ByteWidth = sizeof(Vertex) * nrOfVertices;
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags = 0;
	vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the vertex data.
	vertexData.pSysMem = this->vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create vertex buffer
	hr = device->CreateBuffer(&vertexBuffDesc, &vertexData, &this->vertexBuffer);
	if (FAILED(hr))
	{
		return false;
	}
	
	return true;
}

bool Object::render(ID3D11DeviceContext* deviceContext)
{
	UINT stride;
	UINT offset;

	stride = sizeof(Vertex);
	offset = 0;

	// Set the vertex buffer active in the Input Assembler.
	deviceContext->IASetVertexBuffers(0, 1, &this->vertexBuffer, &stride, &offset);

	// Set the type of primitive for this vertex buffer.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw
	deviceContext->Draw(nrOfVertices, 0);
}