#include "App.h"

App::App( std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer ) noexcept :
	window( std::move( window ) ),
	renderer( std::move( renderer ) )
{
}

WPARAM App::MainLoop() const
{
	MSG msg;
	//this->window.Show( SW_SHOW );
	bool running = this->window->Show( SW_SHOW );

	//Vertex vertexBufferData[] = {
	//	{DirectX::XMFLOAT3( 1.0f, 1.0f, 0.0f ), DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f ) } ,
	//	{DirectX::XMFLOAT3( -1.0f, 1.0f, 0.0f ), DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f ) },
	//	{DirectX::XMFLOAT3( 0.0f, -1.0f, 0.0f ), DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f )}
	//};

	while ( running )
	{
		if ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			if ( msg.message == WM_QUIT )
			{
				running = false;
			}
		}
		renderer->Update();
		renderer->Render();
	}

	return 0;
}

void App::SetVertexBufferSize(const UINT size)
{
	vertexBufferSize = size;
}

UINT App::GetVertexBufferSize()
{
	return vertexBufferSize;
}

void App::SetStagingBufferSize(UINT size)
{
	stagingBufferSize = size;
}

UINT App::GetStagingBufferSize()
{
	return stagingBufferSize;
}

UINT App::vertexBufferSize = 1024;
UINT App::stagingBufferSize = 1024;
