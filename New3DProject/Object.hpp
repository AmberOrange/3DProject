#pragma once
#include "Mesh.hpp"

struct Face
{
	int nrOfIndices;

	vector<int> indPos;
	vector<int> indTC;
	vector<int> indNor;
};

class Object : public Mesh
{
public:

	Object();
	Object(string fileName, bool RHCoordSys = false);
	~Object();

	bool Initialize(ID3D11Device* device);

	// Load from .obj file.
	bool loadFromFile(string fileName);

	virtual Mesh *Clone();
private:

	string fileName;

	// Extract information from lines.
	XMFLOAT3 getAsVec3(string line);
	XMFLOAT2 getAsVec2(string line);
	Face getAsFace(string line);

	// Vector math
	XMFLOAT3 cross(XMFLOAT3 vec1, XMFLOAT3 vec2);
	XMFLOAT3 getNormal(XMFLOAT3 pos1, XMFLOAT3 pos2, XMFLOAT3 pos3);
	XMFLOAT3 getVecBetween(XMFLOAT3 pos1, XMFLOAT3 pos2);
	float getVecLength(XMFLOAT3 vec);


};