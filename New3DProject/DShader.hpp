#ifndef DSHADER_HPP
#define DSHADER_HPP

#define GBUFFER_COUNT 4

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using namespace DirectX;
using namespace std;

class DShader
{
public:
	DShader();
	DShader(const DShader &other);
	virtual ~DShader();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, IDXGISwapChain *swapChain, int screenWidth, int screenHeight);
	bool Frame(ID3D11DeviceContext* deviceContext,
		XMMATRIX &worldMatrix,
		XMMATRIX &viewMatrix,
		XMMATRIX &projectionMatrix);

	// Is the following function necessary? Why not just draw directly in the Object Class?
	//bool DrawGeometryBuffers(ID3D11DeviceContext* deviceContext, int vertexCount); 

	bool FinalizeBackBuffer(ID3D11DeviceContext *deviceContext);

private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11DepthStencilState* m_depthStencilState;

	ID3D11VertexShader *m_gshader_vertex;
	ID3D11InputLayout *m_gshader_input;
	ID3D11PixelShader *m_gshader_pixel;

	ID3D11VertexShader *m_deferred_vertex;
	ID3D11InputLayout *m_deferred_input;
	ID3D11PixelShader *m_deferred_pixel;

	ID3D11VertexShader *m_test_vertex;
	ID3D11InputLayout *m_test_input;
	ID3D11PixelShader *m_test_pixel;

	ID3D11Texture2D *m_gBufferTex[GBUFFER_COUNT];
	ID3D11RenderTargetView *m_gRTV[GBUFFER_COUNT];
	ID3D11ShaderResourceView *m_gSRV[GBUFFER_COUNT];

	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer *m_matrixBuffer;
	ID3D11Buffer *m_screenQuad;

	bool CreateShaders(ID3D11Device *device, HWND hwnd);

	bool CreateBuffers(ID3D11Device *device, ID3D11DeviceContext* deviceContext, HWND hwnd, IDXGISwapChain *swapChain, int screenWidth, int screenHeight);

	bool CreateSamplerState(ID3D11Device *device, ID3D11DeviceContext* deviceContext, HWND hwnd);
	bool CreateBackBuffer(ID3D11Device *device, HWND hwnd, IDXGISwapChain *swapChain);
	bool CreateGeometryBuffers(ID3D11Device *device, HWND hwnd, int screenWidth, int screenHeight);
	bool CreateConstantBuffers(ID3D11Device *device, HWND hwnd);
	bool GenerateScreenQuad(ID3D11Device *device, HWND hwnd);

	void ShaderBlobMessage(ID3DBlob *errorMessage, HWND hwnd, WCHAR *shaderFilename);

	void EmptyBuffers(ID3D11DeviceContext *deviceContext);
	bool SetMatrixBuffer(ID3D11DeviceContext *deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix);
	void SetupFirstPass(ID3D11DeviceContext* deviceContext);
	void SetupBackBufferPass(ID3D11DeviceContext* deviceContext);
};

#endif /*DSHADER_HPP*/