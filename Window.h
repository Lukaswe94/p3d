#pragma once
#include "P3dWin.h" 

class Window
{
	public:
	Window() = default;
	//Win32Window( const Win32Window& wnd );
	Window( HINSTANCE hInstance, LPCWSTR pClassname );
	~Window();
	BOOL __stdcall Show( int nCmdShow );
	LPCWSTR GetName() const;
	HWND GetHandle() const;
	HINSTANCE GetInstance() const;
	RECT GetWindowRect() const;
	int ShowErrorWindow( LPCWSTR text, LPCWSTR caption, UINT type );

	private:
	static LRESULT HandleMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HWND hWnd;
	LPCWSTR Classname;
	HINSTANCE hInstance;
};

