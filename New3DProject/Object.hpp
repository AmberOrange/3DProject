#pragma once
#include <fstream>
#include <vector>
#include <d3d11.h>
#include <xstring>
#include <DirectXMath.h>


using namespace DirectX;
using namespace std;
typedef unsigned int UINT;

struct Vertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 TC;
	XMFLOAT3 Normal;
};

/*		*** HOW TO USE ***
*	1. Load object from .obj.
*	2. Create buffers
*	3. Render
*/

class Object
{
public:

	Object();
	~Object();

	bool loadObj(wstring filename);
	bool createBuffer(ID3D11Device* device);
	bool render(ID3D11DeviceContext* deviceContext);

protected:

	UINT* indices;
	Vertex* vertices;
	ID3D11Buffer* vertexBuffer;

	int nrOfVertices;
};