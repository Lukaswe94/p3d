
#include "Renderer/D3d11Renderer.h"
#ifdef _WIN32
// Use discrete GPU by default.

#include "P3dWin.h" 
#include <cstdio>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "App.h"
#include <d3d11.h>
#include <dxgi.h>
#include <vector>
#include <DirectXMath.h>
#include "FileInfo.h"
#include <wrl.h>
#include "Window.h"

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")

extern "C" {
	__declspec( dllexport ) DWORD NV_OPTIMUS_ENABLEMENT = 0x00000001;
	__declspec( dllexport ) int AMD_POWER_XPRESS_REQUEST_HIGH_PERFORMANCE = 1;
}
#endif


int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{

#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	const auto pClassname = L"p3dWindow";
	const std::shared_ptr<Window> wnd( new Window( hInstance, pClassname ));

	IDXGIFactory1* factory;
	HRESULT hr = CreateDXGIFactory1( __uuidof( factory ), reinterpret_cast< void** >( &factory ) );
	assert( SUCCEEDED( hr ) );
	IDXGIAdapter* adapter;
	if ( factory->EnumAdapters( 0, &adapter ) != S_OK )
	{
		OutputDebugString( L"Error enumerating DXGI Adapters" );
		return wnd->ShowErrorWindow( L"Error enumerating DXGI Adapters", L"Error", MB_OK );
	}

	DXGI_ADAPTER_DESC desc;
	hr = adapter->GetDesc( &desc );
	if ( hr == S_OK )
	{
		OutputDebugString( desc.Description );
	}

	const std::shared_ptr<D3d11Renderer> renderer( new D3d11Renderer(wnd) );
	App app( wnd, renderer);
	auto wparam = app.MainLoop();
	OutputDebugString(std::to_wstring(wparam).c_str());
	hr = renderer->GetDebugInterface()->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );

	return 0;
}
