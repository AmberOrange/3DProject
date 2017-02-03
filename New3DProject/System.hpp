#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include "Input.hpp"
//#include "DInput.hpp"
#include "Graphics.hpp"

class System
{
public:
	System();
	System(const System&);
	~System();

	bool Initialize();
	void Run();
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	Input m_input;
	Graphics m_graphics;

	bool Frame();
	void InitializeWindows(int &, int &);
	void ShutdownWindows();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Input* InputHandle = 0;

#endif /* SYSTEM_HPP */