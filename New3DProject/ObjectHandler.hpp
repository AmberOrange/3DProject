#ifndef MODEL_HPP
#define MODEL_HPP

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <sstream>
#include <vector>

ID3D11Buffer* meshVertBuff;
ID3D11Buffer* meshIndexBuff;
//XMMATRIX meshWorld;
int meshSubsets = 0;

using namespace DirectX;
using namespace std;

struct Face
{
	int vIndex1, vIndex2, vIndex3;
	int tIndex1, tIndex2, tIndex3;
	int nIndex1, nIndex2, nIndex3;
};
class ObjectHandler
{
public:
	struct SurfaceMaterial
	{
		std::wstring matName;
		XMFLOAT4 difColor;
		int texArrayIndex;
		bool hasTexture;
		bool transparent;
	};

	ObjectHandler();
	ObjectHandler(const ObjectHandler &);
	~ObjectHandler();

	bool Initialize(ID3D11Device*);
	void Draw(ID3D11DeviceContext*);

	int GetIndexCount();

	//Define LoadObjModel function after we create surfaceMaterial structure
	bool LoadObjModel(std::string filename,        //.obj filename
		ID3D11Buffer** vertBuff,            //mesh vertex buffer
		ID3D11Buffer** indexBuff,            //mesh index buffer
		std::vector<int>& subsetIndexStart,        //start index of each subset
		std::vector<int>& subsetMaterialArray,        //index value of material for each subset
		std::vector<SurfaceMaterial>& material,        //vector of material structures
		int& subsetCount,                //Number of subsets in mesh
		bool isRHCoordSys,                //true if model was created in right hand coord system
		bool computeNormals);                //true to compute the normals, false to use the files normals

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
	bool InitializeBuffersNEW(ID3D11Device*);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadMaterial(string fileName, vector<MaterialType> &materialList);
	bool LoadTextureFromFile(string fileName);
};

#endif /* ObjectHandler_HPP */