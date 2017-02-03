#ifndef INPUT_HPP
#define INPUT_HPP

#include <Windows.h>

// TO BE SWAPPED WITH DIRECT INPUT

class Input
{
public:
	Input();
	Input(const Input &);
	~Input();

	void Initialize();

	void Frame(HWND hwnd);

	void KeyDown(unsigned int);
	void KeyUp(unsigned int);

	bool IsKeyDown(unsigned int);

	int GetMouseX();
	int GetMouseY();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool m_keys[256];
	int m_xmouse, m_ymouse, m_xold, m_yold;

	LRESULT H_KeyDown(HWND hwnd, WPARAM wparam, LPARAM lparam);
	LRESULT H_KeyUp(HWND hwnd, WPARAM wparam, LPARAM lparam);
	LRESULT H_MouseMove(HWND hwnd, WPARAM wparam, LPARAM lparam);
};

#endif /* INPUT_HPP */