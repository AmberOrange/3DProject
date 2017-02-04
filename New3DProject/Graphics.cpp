#include "Graphics.hpp"

Graphics::Graphics()
{

}

Graphics::Graphics(const Graphics & m_graphics)
{
}

Graphics::~Graphics()
{
}

bool Graphics::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Initialize the Direct3D object.
	result = m_Direct3D.Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera.SetPosition(0.0f, 0.0f, -5.0f);

	/*if (!m_Drawable.Initialize(m_Direct3D.GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
	*/

	CreateWICTextureFromFile(m_Direct3D.GetDevice(), L"Isaac.png", nullptr, &m_resource);

	if (!m_MeshHandler.Initialize(m_Direct3D.GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	if (!m_DShader.Initialize(m_Direct3D.GetDevice(), m_Direct3D.GetDeviceContext(), hwnd, m_Direct3D.GetSwapChain(), screenWidth, screenHeight))
	{
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Graphics::Frame(DInput &input, double time)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	m_Camera.Frame(input, time);
	// Generate the view matrix based on the camera's position.
	m_Camera.Render();

	m_Direct3D.GetWorldMatrix(worldMatrix);
	m_Camera.GetViewMatrix(viewMatrix);
	m_Direct3D.GetProjectionMatrix(projectionMatrix);

	m_DShader.Frame(m_Direct3D.GetDeviceContext(), worldMatrix, viewMatrix, projectionMatrix);
	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool Graphics::Render()
{
	m_Direct3D.GetDeviceContext()->PSSetShaderResources(0, 1, &m_resource);
	m_MeshHandler.DrawAll(m_Direct3D.GetDeviceContext());
	//m_Drawable.Draw(m_Direct3D.GetDeviceContext());

	m_DShader.FinalizeBackBuffer(m_Direct3D.GetDeviceContext());

	// Present the rendered scene to the screen.
	m_Direct3D.EndScene();

	return true;
}
