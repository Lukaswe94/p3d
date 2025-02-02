#include "Window.h"

Window::Window( Window&& wnd ) noexcept
	: hWnd( std::move( wnd.hWnd ) )
	, classname( std::move( wnd.classname ) )
	, hInstance( std::move( wnd.hInstance ) )
{
}

Window::Window( HINSTANCE hInstance, LPCWSTR pClassname )
{
	this->classname = pClassname;
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
		throw std::exception( "Error registering Windowclass" );
	}

	//create window instance
	this->hWnd = CreateWindowEx(
		WS_EX_LEFT,
		GetName(),
		L"Window",
		WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_VISIBLE | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		GetInstance(),
		nullptr
	);


	this->hMenu = CreateMenu();
	MENUITEMINFO menuItemInfo = { 0 };
	menuItemInfo.cbSize = sizeof( MENUITEMINFO );
	menuItemInfo.fMask = MIIM_TYPE | MIIM_ID;
	menuItemInfo.fType = MFT_STRING;
	menuItemInfo.fState = MFS_DEFAULT;
	menuItemInfo.dwTypeData = const_cast< wchar_t* >( L"Test" );
	InsertMenuItem( hMenu, 0, true, &menuItemInfo );
	SetMenu( this->hWnd, this->hMenu );
}

Window::~Window()
{
	UnregisterClass( this->classname, this->hInstance );
}

BOOL __stdcall Window::Show( int nCmdShow ) const
{
	return ShowWindow( this->hWnd, nCmdShow );
}

LPCWSTR Window::GetName() const
{
	return this->classname;
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
	BOOL rv = GetClientRect( this->GetHandle(), &rect );
	if ( !rv ) {
		throw;
	}
	return rect;
}

long Window::GetWidth() const
{
	RECT rect = GetWindowRect();
	return rect.right - rect.left;
}

long Window::GetHeight() const
{
	RECT rect = GetWindowRect();
	return rect.bottom - rect.top;
}
int Window::ShowErrorWindow( LPCWSTR text, LPCWSTR caption, UINT type ) const
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
		if ( const HMENU hMenu = GetMenu( hWnd ); hMenu != nullptr )
		{
			DestroyMenu( hMenu );
		}
		DestroyWindow( hWnd );
		break;
	case WM_KEYDOWN:
		s = std::format( "{}", static_cast< char >( wParam ) );
		stemp = std::wstring( s.begin(), s.end() );
		SetWindowText( hWnd, stemp.c_str() );
		break;
	default:
		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	return 0;
}
