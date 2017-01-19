#ifndef MODEL_HPP
#define MODEL_HPP

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>
#include <sstream>

using namespace DirectX;
using namespace std;

class Model
{
public:
	Model();
	Model(const Model &);
	~Model();

	bool Initialize(ID3D11Device*);
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	struct MaterialType
	{
		XMFLOAT3 ambient;
		XMFLOAT3 diffuse;
		XMFLOAT3 specular;

		float specularExp;
		float transparency;

	};

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;

	bool InitializeBuffers(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadMaterial(string fileName, vector<MaterialType> &materialList);
	bool LoadTextureFromFile(string fileName);
};

#endif /* MODEL_HPP */