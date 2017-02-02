#include "Mesh.hpp"

Mesh::Mesh()
{
	this->RHCoordSys = false;
	this->hasTexcoords = false;
	this->faceDefAsTriangles = true;

	this->nrOfVertices = 0;
	this->vertices = nullptr;
}

Mesh::~Mesh()
{
	if (this->vertices != nullptr)
		delete[]this->vertices;
}

bool Mesh::loadFromFile(string fileName)
{
	ifstream fileIn(fileName, ifstream::in);
	
	int nrOfPos;
	int nrOfTC;
	int nrOfNor;
	int nrOfFaces;
	string line;
	bool hasNormals;
	XMFLOAT3 tempPos;
	XMFLOAT2 tempTC;
	XMFLOAT3 tempNor;
	Face tempFace;
	vector<XMFLOAT3> vPos;
	vector<XMFLOAT2> vTC;
	vector<XMFLOAT3> vNor;
	vector<unsigned int> indPos;
	vector<unsigned int> indTC;
	vector<unsigned int> indNor;

	nrOfPos = 0;
	nrOfTC = 0;
	nrOfNor = 0;
	nrOfFaces = 0;
	hasNormals = true;

	if (fileIn.is_open())
	{
		while (fileIn)
		{
			line = "";
			getline(fileIn, line);

			// Check first char in the line.
			switch (line[0])
			{
			case 'v': // Vertex...

				if (line[1] == ' ') // Position.
				{
					tempPos = getAsVec3(line);

					vPos.push_back(tempPos);
					nrOfPos++;
				}

				else if (line[1] == 't') // Texture coordinate.
				{
					this->hasTexcoords = true;
					
					tempTC = getAsVec2(line);

					vTC.push_back(tempTC);
					nrOfTC++;

				}

				else if (line[1] == 'n') // Normal.
				{
					hasNormals = true;

					tempNor = getAsVec3(line);

					vNor.push_back(tempNor);
					nrOfNor++;
				}

				break;

			case 'f': // Face.

				if (line[1] == ' ')
				{
					tempFace = getAsFace(line);

					// Face defined as quad instead of triangle.
					if (faceDefAsTriangles && tempFace.indNor[3] != 0)
						faceDefAsTriangles = false;

					// Face may store 4 values per Pos/TC/Nor.
					// If the 4th is not used, it will have the
					// value 0 by default.
					for (int i = 0; i < 4; i++)
					{
						indPos.push_back(tempFace.indPos[i]);
						indTC.push_back(tempFace.indTC[i]);
						indNor.push_back(tempFace.indNor[i]);
					}

					if (faceDefAsTriangles)
						this->nrOfVertices += 3;
					else
						this->nrOfVertices += 4;

					nrOfFaces++;
				}

				break;
			} // switch

		} // while
	}
	else // File could not open.
	{
		return false;
	}
	fileIn.close();

	// Store values in vertices array.
	this->vertices = new Vertex[this->nrOfVertices];
	for (int i = 0; i < this->nrOfVertices; i++)
	{
		this->vertices[i].Position = vPos[indPos[i] - 1];

		if (this->hasTexcoords)
			this->vertices[i].Texcoord = vTC[indTC[i] - 1];

		if (hasNormals)
			vertices[i].Normal = vNor[indNor[i] - 1];
	}

	// Compute Normals if not stored in obj file.
	if (!hasNormals)
	{
		int jump;

		if (faceDefAsTriangles)
			jump = 3;
		else // Quad
			jump = 4;

		for (int i = 0; i < this->nrOfVertices; i += jump)
		{
			tempNor = getNormal(this->vertices[i].Position, this->vertices[i + 1].Position, this->vertices[i + 2].Position);
		
			for (int j = i; j < i + jump; j++)
			{
				this->vertices[j].Normal = tempNor;
				nrOfNor++;
			}
		}
	}

	// Expand array
	if (!faceDefAsTriangles)
	{	
		int newSize;
		int sampler;
		int facePart;
		Vertex *newVertices;
		
		// If faces store quads, two more
		// vertices will be added per face.
		newSize = this->nrOfVertices + 2 * nrOfFaces;
		sampler = 0;
		facePart = 1;

		newVertices = new Vertex[newSize];

		// Fill new vertex array.
		for (int i = 0; i < newSize; i++)
		{
			newVertices[i] = this->vertices[sampler];

			if (facePart == 3)
				sampler--;
			else
				sampler++;

			if (facePart == 4)
				facePart = 0; // Reset

			facePart++;		
		}

		// Switch to new array and delete old.
		delete[]this->vertices;
		this->vertices = newVertices;
		newVertices = nullptr;
	}
}

XMFLOAT3 Mesh::getAsVec3(string line)
{
	stringstream ss(line);
	XMFLOAT3 returnVec;
	char pop1, pop2;

	if (ss)
	{
		// Get rid of "vo " or "vn" in line
		// to only work with the values.
		pop1 = ss.get();
		pop2 = ss.get();

		if (pop1 == 'v' && ( pop2 == ' ' || pop2 == 'n'))
		{
			ss >> returnVec.x >> returnVec.y >> returnVec.z;
		}
	}

	return returnVec;
}

XMFLOAT2 Mesh::getAsVec2(string line)
{
	stringstream ss(line);
	XMFLOAT2 returnVec;
	char pop1, pop2;

	if (ss)
	{
		// Get rid of "vt" in line
		// to only work with the values.
		pop1 = ss.get();
		pop2 = ss.get();

		if (pop1 == 'v' && pop2 == 't')
		{
			ss >> returnVec.x >> returnVec.y;
		}
	}

	return returnVec;
}

Face Mesh::getAsFace(string line)
{
	stringstream strToInt, ss(line);

	int whichPart;		// Current values is the index of Position, Texcoord or Normal.
	int vertCount;		// Current vertex. 
	string subSS;		// Extract one of three/four vertices at a time from the line.
	string vertPart;	// Used to get current values for the parts of current vertex.
	char pop1, pop2;	// Pop char.
	Face returnFace;

	vertCount = 0;
	subSS = "";

	if (ss)
	{
		// Get rid of "f " in line
		// to only work with the values.
		pop1 = ss.get();
		pop2 = ss.get();

		if (pop1 == 'f' && pop2 == ' ')
		{
			ss >> subSS;

			while (subSS != "")
			{
				whichPart = 0;
				vertPart = "";

				for (int i = 0; i < subSS.length(); i++)
				{
					// Fills vertPart with index value char by char.
					if (subSS[i] != '/')
						vertPart += subSS[i];

					// When vertPart is completed.
					if (subSS[i] == '/' || i == subSS.length() - 1)
					{
						strToInt.clear();
						strToInt << vertPart;

						if (whichPart == 0)
							strToInt >> returnFace.indPos[vertCount];
						else if (whichPart == 1)
							strToInt >> returnFace.indTC[vertCount];
						else if (whichPart == 2)
							strToInt >> returnFace.indNor[vertCount];

						whichPart++;
						vertPart = "";
					}
				} // end for

				vertCount++;

				subSS = "";
				ss >> subSS;
			} // end while
		}
	} // end ss

	return returnFace;
}

XMFLOAT3 Mesh::cross(XMFLOAT3 vec1, XMFLOAT3 vec2)
{
	return XMFLOAT3(
		vec1.y * vec2.z - vec1.z * vec2.y,
		vec1.z * vec2.x - vec1.x * vec2.z,
		vec1.x * vec2.y - vec1.y * vec2.x);
}

XMFLOAT3 Mesh::getNormal(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3)
{
	XMFLOAT3 vec1 = getVecBetween(pos1, pos2);
	XMFLOAT3 vec2 = getVecBetween(pos1, pos3);
	XMFLOAT3 normal;
	float length;

	if (!this->RHCoordSys)
		normal = cross(vec1, vec2);
	else
		normal = cross(vec2, vec1);

	length = getVecLength(normal);

	// Normalize
	normal.x /= length;
	normal.y /= length;
	normal.z /= length;

	return normal;
}

XMFLOAT3 Mesh::getVecBetween(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	return XMFLOAT3(pos2.x - pos1.x, pos2.y - pos1.y, pos2.z - pos1.z);
}

float Mesh::getVecLength(XMFLOAT3 vec)
{
	return sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
}