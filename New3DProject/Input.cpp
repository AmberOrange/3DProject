#include "Input.hpp"

Input::Input()
{
	m_xmouse = 0;
	m_ymouse = 0;
	m_xold = 0;
	m_yold = 0;
}

Input::Input(const Input & m_input)
{
}

Input::~Input()
{
}

void Input::Initialize()
{
	int i;


	// Initialize all the keys to being released and not pressed.
	for (i = 0; i<256; i++)
	{
		m_keys[i] = false;
	}

	return;
}

void Input::Frame(HWND hwnd)
{
	RECT rect;
	POINT tl;

	m_xold = m_xmouse;
	m_yold = m_ymouse;

	m_xmouse = 0;
	m_ymouse = 0;

	GetClientRect(hwnd, &rect);
	tl.x = rect.top;
	tl.y = rect.left;
	ClientToScreen(hwnd, &tl);

	SetCursorPos((tl.x + rect.right / 2), (tl.y + rect.bottom / 2));
}


void Input::KeyDown(unsigned int input)
{
	// If a key is pressed then save that state in the key array.
	m_keys[input] = true;
	return;
}


void Input::KeyUp(unsigned int input)
{
	// If a key is released then clear that state in the key array.
	m_keys[input] = false;
	return;
}


bool Input::IsKeyDown(unsigned int key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keys[key];
}

int Input::GetMouseX()
{
	return m_xold;
}

int Input::GetMouseY()
{
	return m_yold;
}

LRESULT Input::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	POINTS p;
	switch (umsg)
	{
		// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		return this->H_KeyDown(hwnd, wparam, lparam);
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		return this->H_KeyUp(hwnd, wparam, lparam);
	}

	case WM_MOUSEMOVE:
	{
		return this->H_MouseMove(hwnd, wparam, lparam);
	}

	// Any other messages send to the default message handler as our application won't make use of them.
	default:
	{
		return DefWindowProc(hwnd, umsg, wparam, lparam);
	}
	}
}

LRESULT Input::H_KeyDown(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	this->KeyDown((unsigned int)wparam);
	return 0;
}

LRESULT Input::H_KeyUp(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	this->KeyUp((unsigned int)wparam);
	return 0;
}

LRESULT Input::H_MouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam)
{
	RECT rect;
	POINTS ps;
	POINT p, tl, rb;
	int x, y;

	ps = MAKEPOINTS(lparam);
	p.x = ps.x;
	p.y = ps.y;
	ClientToScreen(hwnd, &p);

	GetClientRect(hwnd, &rect);
	tl.x = rect.top;
	tl.y = rect.left;
	ClientToScreen(hwnd, &tl);

	x = p.x - (tl.x + rect.right / 2);
	y = p.y - (tl.y + rect.bottom / 2);

	if (abs(x) > abs(this->m_xmouse))
		this->m_xmouse = x;
	if (abs(y) > abs(this->m_ymouse))
		this->m_ymouse = y;

	//SetCursorPos((tl.x + rect.right / 2), (tl.y + rect.bottom / 2));
	return 0;
}
