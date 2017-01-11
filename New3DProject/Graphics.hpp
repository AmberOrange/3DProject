#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <Windows.h>

#include "D3D.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "DInput.hpp"

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
	Model m_Model;
	Shader m_ColorShader;

};

#endif /* GRAPHICS_HPP */