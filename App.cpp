#include "App.h"

App::App( Window& window, Renderer& renderer ) noexcept : renderer( std::move( renderer ) ), window( std::move( window ) )
{
}

WPARAM App::MainLoop()
{
	MSG msg;
	this->window.Show( SW_SHOW );
	bool running = true;
	HRESULT rv = S_OK;
	while ( running )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			if ( msg.message == WM_QUIT )
			{
				running = false;
			}
		}
		renderer.Render();
	}

	return 0;
}

App::~App()
{
}

