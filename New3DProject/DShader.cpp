#include "DShader.hpp"

DShader::DShader()
{
	// Just for good practise, set all pointers to null

	m_gshader_vertex = nullptr;
	m_gshader_input = nullptr;
	m_gshader_pixel = nullptr;

	m_deferred_vertex = nullptr;
	m_deferred_input = nullptr;
	m_deferred_pixel = nullptr;

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		m_gBufferTex[i] = nullptr;
		m_gRTV[i] = nullptr;
		m_gSRV[i] = nullptr;
	}
}

DShader::DShader(const DShader & other)
{
	// Intentionally left empty
}

DShader::~DShader()
{
	// Release everything
	m_gshader_vertex->Release();
	m_gshader_input->Release();
	m_gshader_pixel->Release();

	m_deferred_vertex->Release();
	m_deferred_input->Release();
	m_deferred_pixel->Release();

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		if(m_gBufferTex[i] != nullptr)
			m_gBufferTex[i]->Release();
		if(m_gRTV[i] != nullptr)
			m_gRTV[i]->Release();
		if(m_gSRV[i] != nullptr)
			m_gSRV[i]->Release();
	}

}

bool DShader::Initialize(ID3D11Device *device, ID3D11DeviceContext* deviceContext, HWND hwnd, IDXGISwapChain *swapChain, int screenWidth, int screenHeight)
{
	// Create the Shaders
	if (!CreateShaders(device, hwnd))
		return false;

	// Create Geometry Buffers
	if (!CreateBuffers(device, deviceContext, hwnd, swapChain, screenWidth, screenHeight))
		return false;

	return true;
}

bool DShader::Frame(ID3D11DeviceContext *deviceContext, XMMATRIX &worldMatrix, XMMATRIX &viewMatrix, XMMATRIX &projectionMatrix)
{
	// Clear all buffers
	EmptyBuffers(deviceContext);

	// Update Constant Buffers
	if (!SetMatrixBuffer(deviceContext, worldMatrix, viewMatrix, projectionMatrix))
		return false;

	// Setup shaders for the First Pass
	SetupFirstPass(deviceContext);

	return true;
}

bool DShader::FinalizeBackBuffer(ID3D11DeviceContext *deviceContext)
{
	UINT stride = sizeof(float) * 5;
	UINT offset = 0;

	// Setup the Shaders for the Finalization
	SetupBackBufferPass(deviceContext);

	// Set the screen quad in IA
	deviceContext->IASetVertexBuffers(0, 1, &m_screenQuad, &stride, &offset);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// Set Render Target to the backbuffer
	// We won't be needing the depth stencil for this
	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);

	// Draw
	deviceContext->Draw(4, 0);

	return true;
}

bool DShader::CreateShaders(ID3D11Device * device, HWND hwnd)
{
	HRESULT result;
	ID3DBlob* errorMessage = nullptr;

	#pragma region gshader_vertex

	ID3DBlob* gshader_vertex_buffer = nullptr;
	WCHAR gshader_vertex_file[] = L"gshader_vertex.hlsl";

	result = D3DCompileFromFile(gshader_vertex_file, NULL, NULL, "VS_main", "vs_5_0", 0, 0,
		&gshader_vertex_buffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			ShaderBlobMessage(errorMessage, hwnd, gshader_vertex_file);
		else
			MessageBox(hwnd, gshader_vertex_file, L"Missing Shader File", MB_OK);
		
		return false;
	}

	result = device->CreateVertexShader(
		gshader_vertex_buffer->GetBufferPointer(), 
		gshader_vertex_buffer->GetBufferSize(), 
		NULL, 
		&m_gshader_vertex);

	if (FAILED(result))
	{
		MessageBox(hwnd, gshader_vertex_file, L"Error when creating", MB_OK);
		return false;
	}

	#pragma region gshader_input

	D3D11_INPUT_ELEMENT_DESC gshader_input_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	result = device->CreateInputLayout(
		gshader_input_desc,
		ARRAYSIZE(gshader_input_desc),
		gshader_vertex_buffer->GetBufferPointer(),
		gshader_vertex_buffer->GetBufferSize(),
		&m_gshader_input);

	if (FAILED(result))
	{
		MessageBox(hwnd, L"gshader_input", L"Error when creating", MB_OK);
		return false;
	}

	#pragma endregion

	gshader_vertex_buffer->Release();
	gshader_vertex_buffer = nullptr;

	#pragma endregion

	#pragma region gshader_pixel

	ID3DBlob* gshader_pixel_buffer = nullptr;
	WCHAR gshader_pixel_file[] = L"gshader_pixel.hlsl";

	result = D3DCompileFromFile(gshader_pixel_file, NULL, NULL, "PS_main", "ps_5_0", 0, 0,
		&gshader_pixel_buffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			ShaderBlobMessage(errorMessage, hwnd, gshader_pixel_file);
		else
			MessageBox(hwnd, gshader_pixel_file, L"Missing Shader File", MB_OK);

		return false;
	}

	result = device->CreatePixelShader(
		gshader_pixel_buffer->GetBufferPointer(),
		gshader_pixel_buffer->GetBufferSize(),
		NULL,
		&m_gshader_pixel);

	if (FAILED(result))
	{
		MessageBox(hwnd, gshader_pixel_file, L"Error when creating", MB_OK);
		return false;
	}

	gshader_pixel_buffer->Release();
	gshader_pixel_buffer = nullptr;

	#pragma endregion

	#pragma region deferred_vertex

	ID3DBlob* deferred_vertex_buffer = nullptr;
	WCHAR deferred_vertex_file[] = L"deferred_vertex.hlsl";

	result = D3DCompileFromFile(deferred_vertex_file, NULL, NULL, "VS_main", "vs_5_0", 0, 0,
		&deferred_vertex_buffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			ShaderBlobMessage(errorMessage, hwnd, deferred_vertex_file);
		else
			MessageBox(hwnd, deferred_vertex_file, L"Missing Shader File", MB_OK);

		return false;
	}

	result = device->CreateVertexShader(
		deferred_vertex_buffer->GetBufferPointer(),
		deferred_vertex_buffer->GetBufferSize(),
		NULL,
		&m_deferred_vertex);

	if (FAILED(result))
	{
		MessageBox(hwnd, deferred_vertex_file, L"Error when creating", MB_OK);
		return false;
	}

	#pragma region deferred_input

	D3D11_INPUT_ELEMENT_DESC deferred_input_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = device->CreateInputLayout(
		deferred_input_desc,
		ARRAYSIZE(deferred_input_desc),
		deferred_vertex_buffer->GetBufferPointer(),
		deferred_vertex_buffer->GetBufferSize(),
		&m_deferred_input);

	if (FAILED(result))
	{
		MessageBox(hwnd, L"deferred_input", L"Error when creating", MB_OK);
		return false;
	}

	#pragma endregion

	deferred_vertex_buffer->Release();
	deferred_vertex_buffer = nullptr;

	#pragma endregion

	#pragma region deferred_pixel

	ID3DBlob* deferred_pixel_buffer = nullptr;
	WCHAR deferred_pixel_file[] = L"deferred_pixel.hlsl";

	result = D3DCompileFromFile(deferred_pixel_file, NULL, NULL, "PS_main", "ps_5_0", 0, 0,
		&deferred_pixel_buffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			ShaderBlobMessage(errorMessage, hwnd, deferred_pixel_file);
		else
			MessageBox(hwnd, deferred_pixel_file, L"Missing Shader File", MB_OK);

		return false;
	}

	result = device->CreatePixelShader(
		deferred_pixel_buffer->GetBufferPointer(),
		deferred_pixel_buffer->GetBufferSize(),
		NULL,
		&m_deferred_pixel);

	if (FAILED(result))
	{
		MessageBox(hwnd, deferred_pixel_file, L"Error when creating", MB_OK);
		return false;
	}

	deferred_pixel_buffer->Release();
	deferred_pixel_buffer = nullptr;

	#pragma endregion

	#pragma region test_vertex

	ID3DBlob* test_vertex_buffer = nullptr;
	WCHAR test_vertex_file[] = L"test_vertex.hlsl";

	result = D3DCompileFromFile(test_vertex_file, NULL, NULL, "VS_main", "vs_5_0", 0, 0,
		&test_vertex_buffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			ShaderBlobMessage(errorMessage, hwnd, test_vertex_file);
		else
			MessageBox(hwnd, test_vertex_file, L"Missing Shader File", MB_OK);

		return false;
	}

	result = device->CreateVertexShader(
		test_vertex_buffer->GetBufferPointer(),
		test_vertex_buffer->GetBufferSize(),
		NULL,
		&m_test_vertex);

	if (FAILED(result))
	{
		MessageBox(hwnd, test_vertex_file, L"Error when creating", MB_OK);
		return false;
	}

	#pragma region test_input

	D3D11_INPUT_ELEMENT_DESC test_input_desc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	result = device->CreateInputLayout(
		test_input_desc,
		ARRAYSIZE(test_input_desc),
		test_vertex_buffer->GetBufferPointer(),
		test_vertex_buffer->GetBufferSize(),
		&m_test_input);

	if (FAILED(result))
	{
		MessageBox(hwnd, L"test_input", L"Error when creating", MB_OK);
		return false;
	}

	#pragma endregion

	test_vertex_buffer->Release();
	test_vertex_buffer = nullptr;

	#pragma endregion

	#pragma region test_pixel

	ID3DBlob* test_pixel_buffer = nullptr;
	WCHAR test_pixel_file[] = L"test_pixel.hlsl";

	result = D3DCompileFromFile(test_pixel_file, NULL, NULL, "PS_main", "ps_5_0", 0, 0,
		&test_pixel_buffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			ShaderBlobMessage(errorMessage, hwnd, test_pixel_file);
		else
			MessageBox(hwnd, test_pixel_file, L"Missing Shader File", MB_OK);

		return false;
	}

	result = device->CreatePixelShader(
		test_pixel_buffer->GetBufferPointer(),
		test_pixel_buffer->GetBufferSize(),
		NULL,
		&m_test_pixel);

	if (FAILED(result))
	{
		MessageBox(hwnd, test_pixel_file, L"Error when creating", MB_OK);
		return false;
	}

	test_pixel_buffer->Release();
	test_pixel_buffer = nullptr;

	#pragma endregion

	return true;
}

bool DShader::CreateBuffers(ID3D11Device *device, ID3D11DeviceContext* deviceContext, HWND hwnd, IDXGISwapChain *swapChain, int screenWidth, int screenHeight)
{
	if (!CreateSamplerState(device, deviceContext, hwnd))
		return false;

	if(!CreateBackBuffer(device, hwnd, swapChain))
		return false;

	if(!CreateGeometryBuffers(device, hwnd, screenWidth, screenHeight))
		return false;

	if(!CreateConstantBuffers(device, hwnd))
		return false;

	if(!GenerateScreenQuad(device, hwnd))
		return false;

	return true;
}

bool DShader::CreateSamplerState(ID3D11Device * device, ID3D11DeviceContext* deviceContext, HWND hwnd)
{
	HRESULT result;
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.MaxAnisotropy = 8;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	samplerDesc.MinLOD = 0;
	samplerDesc.MipLODBias = 0;

	result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to create Sampler State at m_sampleState", L"Unable to CreateSamplerState", MB_OK);
		return false;
	}

	deviceContext->PSSetSamplers(0, 1, &m_sampleState);

	return true;
}

bool DShader::CreateBackBuffer(ID3D11Device *device, HWND hwnd, IDXGISwapChain *swapChain)
{
	HRESULT result;
	ID3D11Texture2D* backBufferTex;

	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTex);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to get backBufferTex", L"Unable to get buffer", MB_OK);
		return false;
	}

	result = device->CreateRenderTargetView(backBufferTex, NULL, &m_renderTargetView);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to Create m_renderTargetView", L"Unable to Create", MB_OK);
		return false;
	}

	return true;
}

bool DShader::CreateGeometryBuffers(ID3D11Device *device, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResDesc;

	ZeroMemory(&textureDesc, sizeof(textureDesc));
	textureDesc.Width = screenWidth;
	textureDesc.Height = screenHeight;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	ZeroMemory(&renderTargetViewDesc, sizeof(renderTargetViewDesc));
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	ZeroMemory(&shaderResDesc, sizeof(shaderResDesc));
	shaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResDesc.Texture2D.MostDetailedMip = 0;
	shaderResDesc.Texture2D.MipLevels = 1;

	// Create everything for the GBuffers
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	renderTargetViewDesc.Format = textureDesc.Format;
	shaderResDesc.Format = textureDesc.Format;

	//if(!GenerateGeometryBuffers(device, hwnd, &textureDesc, &renderTargetViewDesc, &shaderResDesc, 0)) // POS X Y Z
	//	return false;

	//if (!GenerateGeometryBuffers(device, hwnd, &textureDesc, &renderTargetViewDesc, &shaderResDesc, 2)) // DIF R G B
	//	return false;

	//if (!GenerateGeometryBuffers(device, hwnd, &textureDesc, &renderTargetViewDesc, &shaderResDesc, 3)) // SPEC R G B
	//	return false;


	////textureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	////renderTargetViewDesc.Format = textureDesc.Format;
	////shaderResDesc.Format = textureDesc.Format;
	//if (!GenerateGeometryBuffers(device, hwnd, &textureDesc, &renderTargetViewDesc, &shaderResDesc, 1)) // NOR X Y Z
	//	return false;

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		result = device->CreateTexture2D(&textureDesc, NULL, &m_gBufferTex[i]);
		if (FAILED(result))
		{
			MessageBox(hwnd, L"Unable to Create one of the Textures in m_gBufferTex", L"Unable to Create Texture2D", MB_OK);
			return false;
		}

		result = device->CreateRenderTargetView(m_gBufferTex[i], &renderTargetViewDesc, &m_gRTV[i]);
		if (FAILED(result))
		{
			MessageBox(hwnd, L"Unable to Create one of the RenderTargetViews in m_gRTV", L"Unable to Create RenderTargetView", MB_OK);
			return false;
		}

		result = device->CreateShaderResourceView(m_gBufferTex[i], &shaderResDesc, &m_gSRV[i]);
		if (FAILED(result))
		{
			MessageBox(hwnd, L"Unable to Create one of the ShaderResourceViews in m_gSRV", L"Unable to Create ShaderResourceView", MB_OK);
			return false;
		}

	}

	return true;
}

bool DShader::GenerateGeometryBuffers(ID3D11Device *device, HWND hwnd, D3D11_TEXTURE2D_DESC *textureDesc, D3D11_RENDER_TARGET_VIEW_DESC *renderTargetViewDesc, D3D11_SHADER_RESOURCE_VIEW_DESC *shaderResDesc, int i)
{
	HRESULT result;
	result = device->CreateTexture2D(textureDesc, NULL, &m_gBufferTex[i]);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to Create one of the Textures in m_gBufferTex", L"Unable to Create Texture2D", MB_OK);
		return false;
	}

	result = device->CreateRenderTargetView(m_gBufferTex[i], renderTargetViewDesc, &m_gRTV[i]);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to Create one of the RenderTargetViews in m_gRTV", L"Unable to Create RenderTargetView", MB_OK);
		return false;
	}

	result = device->CreateShaderResourceView(m_gBufferTex[i], shaderResDesc, &m_gSRV[i]);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to Create one of the ShaderResourceViews in m_gSRV", L"Unable to Create ShaderResourceView", MB_OK);
		return false;
	}

	return true;
}

bool DShader::CreateConstantBuffers(ID3D11Device * device, HWND hwnd)
{
	HRESULT result;

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC matrixBufferDesc;

	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to Create the Matrix Buffer at m_matrixBuffer", L"Unable to Create Buffer", MB_OK);
		return false;
	}

	return true;
}

bool DShader::GenerateScreenQuad(ID3D11Device * device, HWND hwnd)
{
	HRESULT result;
	struct QuadStruct
	{
		float x,y,z;
		float u,v;
	};

	QuadStruct vertex[] = {
		-1.0f,	-1.0f, 0.f,
		0.0f,	1.0f,
		-1.0f,	1.0f, 0.f,
		0.0f,	0.0f,
		1.0f,	-1.0f, 0.f,
		1.0f,	1.0f,
		1.0f,	1.0f, 0.f,
		1.0f,	0.0f
	};

	D3D11_BUFFER_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.ByteWidth = sizeof(QuadStruct) * 4;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.StructureByteStride = NULL;
	bufferDesc.MiscFlags = NULL;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertex;

	result = device->CreateBuffer(&bufferDesc, &data, &m_screenQuad);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Unable to Create the Screen Quad", L"Unable to Create the Screen Quad", MB_OK);
		return false;
	}

	return true;
}

void DShader::ShaderBlobMessage(ID3DBlob * errorMessage, HWND hwnd, WCHAR * shaderFilename)
{
	char* compileErrors;
	unsigned long long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

}

void DShader::EmptyBuffers(ID3D11DeviceContext * deviceContext)
{
	float color[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < GBUFFER_COUNT; i++)
	{
		deviceContext->ClearRenderTargetView(m_gRTV[i], color);
	}
	float bcolor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	deviceContext->ClearRenderTargetView(m_renderTargetView, bcolor);
	deviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

bool DShader::SetMatrixBuffer(ID3D11DeviceContext * deviceContext, XMMATRIX & worldMatrix, XMMATRIX & viewMatrix, XMMATRIX & projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX m_worldMatrix = XMMatrixTranspose(worldMatrix);
	XMMATRIX m_viewMatrix = XMMatrixTranspose(viewMatrix);
	XMMATRIX m_projectionMatrix = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer.
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = m_worldMatrix;
	dataPtr->view = m_viewMatrix;
	dataPtr->projection = m_projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(m_matrixBuffer, 0);

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

	return true;
}

void DShader::SetupFirstPass(ID3D11DeviceContext *deviceContext)
{
	deviceContext->VSSetShader(m_gshader_vertex, nullptr, 0);
	deviceContext->IASetInputLayout(m_gshader_input);
	//deviceContext->GSSetShader(m_gshader_geometry, nullptr, 0); // Doesn't exist yet
	deviceContext->PSSetShader(m_gshader_pixel, nullptr, 0);

	deviceContext->OMSetRenderTargets(GBUFFER_COUNT, m_gRTV, m_depthStencilView);
	//deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}

void DShader::SetupBackBufferPass(ID3D11DeviceContext *deviceContext)
{
	ID3D11RenderTargetView* temp[] = { nullptr, nullptr, nullptr, nullptr };
	deviceContext->OMSetRenderTargets(4, temp, nullptr);

	deviceContext->VSSetShader(m_deferred_vertex, nullptr, 0);
	deviceContext->IASetInputLayout(m_deferred_input);
	deviceContext->PSSetShader(m_deferred_pixel, nullptr, 0);

	deviceContext->PSSetShaderResources(0, GBUFFER_COUNT, m_gSRV);

	deviceContext->OMSetRenderTargets(1, &m_renderTargetView, nullptr);
}
