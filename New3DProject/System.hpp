#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

<<<<<<< HEAD
#include "Input.hpp"
//#include "DInput.hpp"
=======
//#include "Input.hpp"
#include "Timer.hpp"
#include "DInput.hpp"
>>>>>>> bdd00f26f199416230860948fb6269d14c9ea122
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

<<<<<<< HEAD
	Input m_input;
=======
	//Input m_input;
	Timer m_timer;
	DInput m_input;
>>>>>>> bdd00f26f199416230860948fb6269d14c9ea122
	Graphics m_graphics;

	bool Frame();
	void InitializeWindows(int &, int &);
	void ShutdownWindows();
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static Input* InputHandle = 0;

#endif /* SYSTEM_HPP */