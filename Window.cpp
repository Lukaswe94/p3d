#include "Window.h"
#include <format>
using namespace std;

//Win32Window::Win32Window( const Win32Window& wnd ) 
//{
//	this->Classname = wnd.GetName();
//	this->hWnd = wnd.GetHandle();
//	this->hInstance = wnd.GetInstance();
//}

Window::Window( HINSTANCE hInstance, LPCWSTR pClassname )
{
	this->Classname = pClassname;
	this->hInstance = hInstance;

	//register window class
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsg;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();

	if ( !RegisterClassEx( &wc ) )
	{
		throw std::exception("Error registering Windowclass");
	}

	//create window instance
	this->hWnd = CreateWindowEx(
		WS_EX_LEFT,
		GetName(),
		L"Window",
		WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE | WS_SYSMENU | WS_MAXIMIZE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		GetInstance(),
		nullptr
	);
}

Window::~Window()
{
	UnregisterClass( this->Classname, this->hInstance );
}

BOOL __stdcall Window::Show( int nCmdShow )
{
	return ShowWindow(this->hWnd, nCmdShow);
}

LPCWSTR Window::GetName() const
{
	return this->Classname;
}

HWND Window::GetHandle() const
{
	return this->hWnd;
}

HINSTANCE Window::GetInstance() const
{
	return this->hInstance;
}

RECT Window::GetWindowRect() const
{
	RECT rect = {};
	GetClientRect(this->GetHandle(), &rect);
	return rect;
}

int Window::ShowErrorWindow(LPCWSTR text, LPCWSTR caption, UINT type )
{
	return MessageBox( GetHandle(), text, caption, type );
}

LRESULT Window::HandleMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	std::string s;
	std::wstring stemp;

	switch ( uMsg )
	{
		case WM_DESTROY:
			PostQuitMessage( 69 );
			break;
		case WM_KEYDOWN:
			s = std::format( "{}", (char)wParam );
			stemp = std::wstring( s.begin(), s.end() );
			SetWindowText( hWnd, stemp.c_str() );
			break;
		default:
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}
