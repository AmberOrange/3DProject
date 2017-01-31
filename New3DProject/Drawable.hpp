#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <d3d11.h>
#include "WICTextureLoader.h"

using namespace DirectX;

class Drawable
{
public:
	Drawable();
	Drawable(const Drawable &other);
	virtual ~Drawable();

	bool Initialize(ID3D11Device *device, HWND hwnd);
	bool Draw(ID3D11DeviceContext *deviceContext);
private:
	struct Vertex
	{
		float px, py, pz;
		float nx, ny, nz;
		float tu, tv;
	};

	ID3D11Buffer *m_vertexbuffer;
	//ID3D11Texture2D *m_texture;
	ID3D11ShaderResourceView *m_resource;
};

#endif /* DRAWABLE_HPP */