#pragma once
#include <fstream>
#include <vector>
#include <xstring>
#include <DirectXMath.h>
#include <sstream>

#include "Object.hpp"

#define CAP 5


using namespace DirectX;
using namespace std;

/*		*** HOW TO USE ***
*	1. Load object from .obj.
*	2. Create buffers
*	3. Render
*/

class MeshHandler
{
private:

	Mesh **meshes;

	int nrOfMeshes;
	int capacity;

	void nullArray();
	void Expand();

public:

	MeshHandler();
	~MeshHandler();

	bool add(Mesh &mesh);

	bool Initialize(ID3D11Device *device, HWND hwnd);
	bool DrawAll(ID3D11DeviceContext *deviceContext);
};