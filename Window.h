#pragma once
#include "P3dWin.h" 

class Window
{
	public:
	Window() = delete;
	Window( Window&& wnd ) noexcept;
	Window( HINSTANCE hInstance, LPCWSTR pClassname );
	~Window();
	BOOL __stdcall Show( int nCmdShow ) const;
	LPCWSTR GetName() const;
	HWND GetHandle() const;
	HINSTANCE GetInstance() const;
	RECT GetWindowRect() const;
	long GetWidth() const;
	long GetHeight() const;
	int ShowErrorWindow( LPCWSTR text, LPCWSTR caption, UINT type ) const;

	private:
	static LRESULT HandleMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HWND hWnd;
	LPCWSTR Classname;
	HINSTANCE hInstance;
};

