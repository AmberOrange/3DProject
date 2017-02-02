#include "MeshHandler.hpp"

MeshHandler::MeshHandler()
{
	this->capacity = CAP;
	this->nrOfMeshes = 0;

	this->meshes = new Mesh*[this->capacity];
	this->nullArray();
}

MeshHandler::~MeshHandler()
{
	this->nullArray();
}

bool MeshHandler::loadFromFile(string filename, bool RHCoordSys)
{

	// ?
	return true;
}

void MeshHandler::nullArray()
{
	for (int i = 0; i < this->capacity; i++)
	{
		// May delete object.
		this->meshes[i] = nullptr;
	}
}

bool MeshHandler::createBuffer(ID3D11Device* device)
{
	/*
	*	FIX:
	*	vertexBuffDesc.ByteWidth
	*	vertexData.pSysMem
	*	deviceContext->Draw
	*/

	D3D11_BUFFER_DESC vertexBuffDesc;
	D3D11_SUBRESOURCE_DATA vertexData;
	HRESULT hr;

	// Clear memory from unwanted data.
	ZeroMemory(&vertexBuffDesc, sizeof(vertexBuffDesc));
	ZeroMemory(&vertexData, sizeof(vertexData));

	// Set up description of the vertex buffer.
	vertexBuffDesc.Usage = D3D11_USAGE_DEFAULT;
	//vertexBuffDesc.ByteWidth = sizeof(Vertex) * nrOfVertices;
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags = 0;
	vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;

	// Give the subresource a pointer to the vertex data.
	//vertexData.pSysMem = vertices;
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

bool MeshHandler::render(ID3D11DeviceContext* deviceContext)
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
	//deviceContext->Draw(this->meshes.nrOfVertices, 0);

	return true;
}