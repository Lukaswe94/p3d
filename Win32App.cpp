#include "Win32App.h"

Win32App::Win32App( Win32Window &window ) noexcept
{
	wnd = window;
}

WPARAM Win32App::MainLoop()
{
	BOOL bRet;
	MSG msg;

	this->wnd.Show( SW_SHOWMAXIMIZED );

	while ( ( bRet = GetMessage( &msg, NULL, 0, 0 ) ) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}

	if ( bRet == -1 )
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}

Win32App::~Win32App()
{
}

