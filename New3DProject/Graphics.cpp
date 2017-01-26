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
	
	if (!m_Model.Initialize(m_Direct3D.GetDevice()))
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	if (!m_Shader.Initialize(m_Direct3D.GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Graphics::Frame(DInput &input)
{
	bool result;

	m_Camera.Frame(input);
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
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;
	
	// Clear the buffers to begin the scene.
	m_Direct3D.BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera.Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Direct3D.GetWorldMatrix(worldMatrix);
	m_Camera.GetViewMatrix(viewMatrix);
	m_Direct3D.GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model.Render(m_Direct3D.GetDeviceContext());

	// Render the model using the color shader.
	result = m_Shader.Frame(m_Direct3D.GetDeviceContext(), m_Model.GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Present the rendered scene to the screen.
	m_Direct3D.EndScene();

	return true;
}
