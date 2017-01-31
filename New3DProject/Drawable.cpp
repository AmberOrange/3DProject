#include "Drawable.hpp"

Drawable::Drawable()
{
	m_vertexbuffer = nullptr;
	m_resource = nullptr;
}

Drawable::Drawable(const Drawable & other)
{
}

Drawable::~Drawable()
{
	m_vertexbuffer->Release();
	m_resource->Release();
}

bool Drawable::Initialize(ID3D11Device *device, HWND hwnd)
{
	HRESULT result;
	Vertex triangleVertices[4] =
	{
		-2.8f, 2.8f, 0.0f,	//v0 pos
		1.0f, 0.0f, 0.0f,	//v0 nor
		0.0f, 0.0f,			//v0 tex


		2.8f, 2.8f, 0.0f,	//v1
		1.0f, 0.0f, 0.0f,	//v1 color
		1.0f, 0.0f,			//v1 tex

		-2.8f, -2.8f, 0.0f,	//v2
		1.0f, 0.0f, 0.0f,	//v2 color
		0.0f, 1.0f,			//v2 tex

		2.8f, -2.8f, 0.0f,	//v3
		1.0f, 0.0f, 0.0f,	//v3 color
		1.0f, 1.0f			//v3 tex
	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * 4;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	result = device->CreateBuffer(&bufferDesc, &data, &m_vertexbuffer);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to get backBufferTex", L"Unable to get buffer", MB_OK);
		return false;
	}

	CreateWICTextureFromFile(device, L"Isaac.png", nullptr, &m_resource);

	return true;
}

bool Drawable::Draw(ID3D11DeviceContext *deviceContext)
{
	UINT stride;
	UINT offset = 0;

	stride = sizeof(Vertex);

	deviceContext->PSSetShaderResources(0, 1, &m_resource);

	deviceContext->IASetVertexBuffers(0, 1, &m_vertexbuffer, &stride, &offset);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	deviceContext->Draw(4, 0);

	return true;
}
