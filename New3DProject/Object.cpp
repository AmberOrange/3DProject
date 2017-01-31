#include "Object.hpp"

bool Object::loadObj(wstring filename, bool RHCoordSys)
{
	HRESULT hr;
	ifstream fileIn;

	// Temporary vectors to be used in vertices.
	vector<XMFLOAT3> vPos;
	vector<XMFLOAT2> vTC;
	vector<XMFLOAT3> vNor;

	// Indices to the temporary vectors.
	vector<UINT> vPosIndex;
	vector<UINT> vTCIndex;
	vector<UINT> vNorIndex;
	int tempInt;

	// Defines if obj file stores texture coordinates and/or normals.
	bool hasTC = false;
	bool hasNorm = false;

	// Variables when storing face data.
	wstring face, vertexDef, vertexPart;
	int triangleCount;
	int facePart;	// Specify if currect value is vPos, cTC or vNor.
	wstringstream ss, wstringToInt;


	// Additional variables used when reading from obj file.
	char checkChar;
	XMFLOAT3 tempV;
	XMFLOAT2 tempTC;
	bool loop = true;

	fileIn.open(filename, ifstream::in);

	if (fileIn.is_open())
	{
		while (loop)
		{
			checkChar = fileIn.get();

			switch (checkChar) // May need to add a case for '\n'
			{
			case '#':	// Ignore line due to comments.
				checkChar = fileIn.get();
				while (checkChar != '\n')
				{
					checkChar = fileIn.get();
				}

				break;

			case 'v':	// Vertex.
				checkChar = fileIn.get();

				if (checkChar == ' ')	// Vertex position
				{
					// Store values.
					fileIn >> tempV.x >> tempV.y >> tempV.z;

					if (RHCoordSys)
					{
						tempV.z *= (-1.0f);
					}
					vPos.push_back(tempV);
				}
				else if (checkChar == 't') // Vertex texcoord
				{
					// Store values.
					fileIn >> tempTC.x >> tempTC.y;

					if (RHCoordSys)
					{
						tempTC.y = 1.0f - tempTC.y;
					}
					vTC.push_back(tempTC);

					// Sence we now know the model includes texcoords.
					hasTC = true;
				}
				else if (checkChar == 'n')
				{
					// Store values.
					fileIn >> tempV.x >> tempV.y >> tempV.z;

					if (RHCoordSys)
					{
						tempV.z *= (-1.0f);
					}
					vNor.push_back(tempV);

					// Sence we now know the model includes normals.
					hasNorm = true;
				}

				break;

			case 'g': // Group, to be continued..
				checkChar = fileIn.get();
				while (checkChar != '\n')
				{
					checkChar = fileIn.get();
				}

				break;

			case 'f':	// Face.
				checkChar = fileIn.get();

				if (checkChar == ' ')
				{
					face = L"";
					triangleCount = 0;

					checkChar = fileIn.get();
					while (checkChar != '\n')
					{
						face += checkChar;			// Add char to line string.
						checkChar = fileIn.get();

						// Add triangle for each space,
						// this is in case the face has
						// three or four faces.
						if (checkChar == ' ')		
						{
							triangleCount++;
						}
					}

					// Set number of triangles to the correct value.
					triangleCount--;

					ss.clear();
					ss << face; // May need to be fixed by "create" the wstringstream here

					if (face.length() > 0)
					{
						for (int i = 0; i < 3; i++) // First triangle.
						{
							ss >> vertexDef; // First values of vPos, (vTC), (vNorm).
							facePart = 0;

							for (int j = 0; j < vertexDef.length(); j++)
							{
								if (vertexDef[j] != '/')
									vertexPart += vertexDef[j];

								if (vertexDef[j] == '/' || j == vertexDef.length() - 1)
								{
									wstringToInt.clear();
									wstringToInt << vertexPart;

									if (facePart == 0) // If vPos
									{
										wstringToInt >> tempInt;
										tempInt--; // Obj start index from 1, we want to start from 0.
										vPosIndex.push_back(tempInt);
									}
									else if (facePart == 1)
									{

									}
								}
							}
						}
					}

				}

			}

		}
	}
	return true;
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

	return true;
}