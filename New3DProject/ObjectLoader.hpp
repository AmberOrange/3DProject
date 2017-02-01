#pragma once
#include <fstream>
#include <vector>
#include <d3d11.h>
#include <xstring>
#include <DirectXMath.h>
#include <sstream>

#include "Mesh.hpp"


using namespace DirectX;
using namespace std;

/*		*** HOW TO USE ***
*	1. Load object from .obj.
*	2. Create buffers
*	3. Render
*/

class ObjectLoader
{
private:

	Mesh* meshes;
	ID3D11Buffer* vertexBuffer;

public:

	ObjectLoader();
	~ObjectLoader();

	bool loadFromFile(string filename, bool RHCoordSys = false);

	bool createBuffer(ID3D11Device* device);
	bool render(ID3D11DeviceContext* deviceContext);

};