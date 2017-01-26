#include "ObjectHandler.hpp"

ObjectHandler::ObjectHandler()
{
	m_vertexBuffer = nullptr;
	m_indexBuffer = nullptr;
}

ObjectHandler::ObjectHandler(const ObjectHandler &)
{
}

ObjectHandler::~ObjectHandler()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
}

bool ObjectHandler::Initialize(ID3D11Device *device)
{
	return InitializeBuffers(device);
}

void ObjectHandler::Render(ID3D11DeviceContext *deviceContext)
{
	RenderBuffers(deviceContext);
}

int ObjectHandler::GetIndexCount()
{
	return m_indexCount;
}

bool ObjectHandler::LoadObjModel(string fileName, vector<XMFLOAT3> &vertices, vector<XMFLOAT2> &texcoords, vector<XMFLOAT3> &normals, vector<Face> &faces, bool rightHanded = false)
{
	ifstream file(fileName);
	string line, special;
	char input;
	istringstream inputString;

	XMFLOAT3 vtx;
	XMFLOAT2 tcd;
	Face fcs;

	file.open(fileName);
	if (file.fail())
		return false;

	// Read from file and continue to read until
	// the end of file is reached.
	while (getline(file, line))
	{
		inputString.str(line);

		// Read in the vertices.
		if (line.substr(0, 2) == "v ")
		{
			inputString >> special >>
				vtx.x >> vtx.y >> vtx.z;
			if (rightHanded)
			{
				// Invert the Z vertex to change to right hand system.
				vtx.z = vtx.z * (-1.0f);
			}
			vertices.vertices.push_back(vtx);
		}

		// Read in the texture uv coordinates.
		else if (line.substr(0, 2) == "vt")
		{
			inputString >> special >>
				tcd.x >> tcd.y;
			if (rightHanded)
			{
				// Invert the V texture coordinates to left hand system.
				tcd.y = 1.0f - tcd.y;
			}
			texcoords.push_back(tcd);
		}

		// Read in the normals.
		else if (line.substr(0, 2) == "vn")
		{
			inputString >> special >>
				vtx.x >> vtx.y >> vtx.z;
			if (rightHanded)
			{
				// Invert the Z normal to change to left hand system.
				vtx.z = vtx.z * (-1.0f);
			}
			normals.push_back(vtx);
		}

		// Read in the faces.
		else if (line.substr(0, 2) == "f ")
		{
			if (!rightHanded)
			{
				inputString >> special;
				inputString >>
					fcs.vIndex1 >> input >>
					fcs.tIndex1 >> input >>
					fcs.nIndex1;
				inputString >>
					fcs.vIndex2 >> input >>
					fcs.tIndex2 >> input >>
					fcs.nIndex2;
				inputString >>
					fcs.vIndex3 >> input >>
					fcs.tIndex3 >> input >>
					fcs.nIndex3;
			}
			else
			{
				inputString >> special;
				inputString >>
					fcs.vIndex3 >> input >>
					fcs.tIndex3 >> input >>
					fcs.nIndex3;
				inputString >>
					fcs.vIndex2 >> input >>
					fcs.tIndex2 >> input >>
					fcs.nIndex2;
				inputString >>
					fcs.vIndex1 >> input >>
					fcs.tIndex1 >> input >>
					fcs.nIndex1;
			}
		}
	}
	file.close();
}

bool ObjectHandler::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

					 // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool ObjectHandler::InitializeBuffersNEW(ID3D11Device* device)
{
	vector<XMFLOAT3>* vertices;
	vector<XMFLOAT2>* texcoords;
	vector<XMFLOAT3>* normals;
	vector<Face>* faces;

	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new vector<XMFLOAT3>;
	texcoords = new vector<XMFLOAT2>;
	normals = new vector<XMFLOAT3>;
	faces = new vector<Face>;

	LoadObjModel("test.obj", *vertices, *texcoords, *normals, *faces);

	// Set the number of vertices in the vertex array.
	m_vertexCount = vertices->size();

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(XMFLOAT3) * m_vertexCount; // May check sizeof
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(Face) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = faces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete vertices;

	delete faces;

	return true;
}

void ObjectHandler::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	return;
}

bool ObjectHandler::LoadMaterial(string fileName, vector<MaterialType> &mList)
{
	ifstream file;
	string sInput, special;
	istringstream iStream;
	MaterialType curMat;

	file.open(fileName);
	if (file.fail() == true)
		return false;

	//file.get(cInput);
	while (getline(file, sInput))
	{
		iStream.str(sInput);
		if (sInput.substr(0, 2) == "Ka")
		{
			iStream >> special >>
				curMat.ambient.x >> curMat.ambient.y >> curMat.ambient.z;
		}
		else if(sInput.substr(0, 2) == "Kd")
		{
			iStream >> special >>
				curMat.diffuse.x >> curMat.diffuse.y >> curMat.diffuse.z;
		}
		else if (sInput.substr(0, 2) == "Ks")
		{
			iStream >> special >>
				curMat.diffuse.x >> curMat.diffuse.y >> curMat.diffuse.z;
		}
	}
}

bool ObjectHandler::LoadTextureFromFile(string fileName)
{
	return false;
}
