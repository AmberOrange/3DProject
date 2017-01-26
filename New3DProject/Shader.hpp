#ifndef SHADER_HPP
#define SHADER_HPP

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class Shader
{
public:
	Shader();
	Shader(const Shader&);
	~Shader();

	bool Initialize(ID3D11Device*, HWND);
	bool Frame(ID3D11DeviceContext*, XMMATRIX &, XMMATRIX &, XMMATRIX &);
	void SetFirstPass(ID3D11DeviceContext*);
	void Render(ID3D11DeviceContext*);

private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11VertexShader* m_gbuffer_vertex;
	ID3D11PixelShader* m_gbuffer_pixel;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;

	bool InitializeShader(ID3D11Device*, HWND);
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool SetConstantBuffer(ID3D11DeviceContext*, XMMATRIX &, XMMATRIX &, XMMATRIX &);

};

#endif /* SHADER_HPP */