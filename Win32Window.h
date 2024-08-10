#pragma once
#include "P3dWin.h" 

class Win32Window
{
	public:
	Win32Window() = default;
	//Win32Window( const Win32Window& wnd );
	Win32Window( HINSTANCE hInstance, LPCWSTR pClassname );
	~Win32Window();
	BOOL __stdcall Show( int nCmdShow );
	LPCWSTR GetName() const;
	HWND GetHandle() const;
	HINSTANCE GetInstance() const;
	RECT GetRect() const;
	int ShowErrorWindow( LPCWSTR text, LPCWSTR caption, UINT type );

	private:
	static LRESULT HandleMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HWND hWnd;
	LPCWSTR Classname;
	HINSTANCE hInstance;
};

