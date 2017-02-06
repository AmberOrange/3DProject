#ifndef DINPUT_HPP
#define DINPUT_HPP

#define DIRECTINPUT_VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include <dinput.h>

class DInput
{
public:
	DInput();
	DInput(const DInput&);
	~DInput();


	bool Initialize(HINSTANCE, HWND, int, int);
	bool Frame();

	bool IsEscapePressed();
	bool IsKeyPressed(unsigned int key);
	void GetMouseLocation(int&, int&);
	void GetMouseMovement(int&, int&);
	DIMOUSESTATE& GetMouseState();
	bool MouseMoved();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	bool m_mouseMoved;
	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
	int m_mouseMoveX, m_mouseMoveY;

	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();
};

#endif /* DINPUT_HPP */