#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <Windows.h>

#include "D3D.hpp"
#include "Camera.hpp"
//#include "Model.hpp"
//#include "Shader.hpp"
#include "MeshHandler.hpp"
#include "Drawable.hpp"
#include "DShader.hpp"
#include "DInput.hpp"

#include "WICTextureLoader.h"

#define FULL_SCREEN false
#define VSYNC_ENABLED true
#define SCREEN_DEPTH 1000.0f
#define SCREEN_NEAR 0.1f

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics &m_graphics);
	~Graphics();

	bool Initialize(int, int, HWND);
	bool Frame(DInput &);

private:
	bool Render();
	D3D m_Direct3D;
	Camera m_Camera;
	//Model m_Model;
	//Shader m_Shader;
	//Drawable m_Drawable;
	MeshHandler m_MeshHandler;
	DShader m_DShader;

	ID3D11ShaderResourceView *m_resource;
};

#endif /* GRAPHICS_HPP */