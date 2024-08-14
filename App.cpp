#include "App.h"

App::App( Window &window, Renderer &renderer ) noexcept : renderer(renderer), wnd(window)
{
}

WPARAM App::MainLoop()
{
	BOOL bRet;
	MSG msg;

	this->wnd.Show( SW_SHOW );

	while ( ( bRet = GetMessage( &msg, NULL, 0, 0 ) ) > 0 )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		renderer.Render();
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

App::~App()
{
}

