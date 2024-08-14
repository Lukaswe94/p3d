
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
#include "Renderer.h"

#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")

extern "C" {
	__declspec( dllexport ) DWORD NvOptimusEnablement = 0x00000001;
	__declspec( dllexport ) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;

int CALLBACK WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
	LPCWSTR pClassname = L"p3dWindow";
	Window wnd = Window( hInstance, pClassname );

	IDXGIFactory1* factory;
	HRESULT hr = CreateDXGIFactory1( __uuidof( factory ), ( void** )( &factory ) );
	IDXGIAdapter* adapter;
	if ( factory->EnumAdapters( 0, &adapter ) != S_OK )
	{
		OutputDebugString( L"Error enumerating DXGI Adapters" );
		wnd.ShowErrorWindow( L"Error enumerating DXGI Adapters", L"Error", MB_OK );
		return -1;
	}
	else
	{
		DXGI_ADAPTER_DESC desc;
		HRESULT hr = adapter->GetDesc( &desc );
		if ( hr == S_OK )
		{
			OutputDebugString( desc.Description );
		}
	}

	//RECT rect = wnd.GetWindowRect();
	//long width = rect.right - rect.left;
	//long height = rect.bottom - rect.top;
	Renderer renderer;
	renderer.Initialize( wnd );
	renderer.CreateShaders();
	App app( wnd, renderer );
	app.MainLoop();
	//DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	//ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
	//swapChainDesc.BufferCount = 1;
	//swapChainDesc.BufferDesc.Width = width;
	//swapChainDesc.BufferDesc.Height = height;
	//swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	//swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//swapChainDesc.OutputWindow = wnd.GetHandle();
	//swapChainDesc.Windowed = true;
	//swapChainDesc.SampleDesc.Count = 1;
	//swapChainDesc.SampleDesc.Quality = 0;

	//ID3D11Device* pDevice;
	//IDXGISwapChain* pSwapChain;
	//D3D_FEATURE_LEVEL featureLevels[ 1 ] = { D3D_FEATURE_LEVEL_11_0 };
	//D3D_FEATURE_LEVEL featureLvl;
	//ID3D11DeviceContext* pDeviceContext;

	//hr = D3D11CreateDeviceAndSwapChain(
	//	nullptr,
	//	D3D_DRIVER_TYPE_HARDWARE,
	//	nullptr,
	//	D3D11_CREATE_DEVICE_DEBUG,
	//	featureLevels,
	//	1,
	//	D3D11_SDK_VERSION,
	//	&swapChainDesc,
	//	&pSwapChain,
	//	&pDevice,
	//	&featureLvl,
	//	&pDeviceContext
	//);
	//uintmax_t size = std::filesystem::file_size( ".\\x64\\Debug\\VertexShader.cso" );
	//FileInfo vsInfo( ".\\x64\\Debug\\VertexShader.cso" );
	//FileInfo psInfo( ".\\x64\\Debug\\PixelShader.cso" );

	//ComPtr<ID3D11VertexShader> pVs;
	//hr = pDevice->CreateVertexShader( vsInfo.GetBytes(), vsInfo.GetSize(), nullptr, pVs.GetAddressOf() );
	//pDeviceContext->VSSetShader( pVs.Get(), nullptr, 0 );

	//ComPtr<ID3D11PixelShader> pPs;
	//hr = pDevice->CreatePixelShader( psInfo.GetBytes(), psInfo.GetSize(), nullptr, pPs.GetAddressOf() );
	//pDeviceContext->PSSetShader( pPs.Get(), nullptr, 0 );

	//ComPtr<ID3D11Texture2D> texture_rgb;
	////hr = pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&texture_rgb );
	//if ( pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&texture_rgb ) != S_OK )
	//{
	//	return 0;
	//}
	//D3D11_TEXTURE2D_DESC texture_desc;
	//texture_rgb->GetDesc( &texture_desc );
	//texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_RENDER_TARGET;

	//ComPtr<ID3D11RenderTargetView> rtv;
	//pDevice->CreateRenderTargetView( texture_rgb.Get(), nullptr, rtv.GetAddressOf() );
	//D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	//ComPtr<ID3D11DepthStencilView> dsv;
	//pDevice->CreateDepthStencilView( texture_rgb.Get(), nullptr, dsv.GetAddressOf() );
	//ComPtr<ID3D11ShaderResourceView> srv;
	//pDevice->CreateShaderResourceView( texture_rgb.Get(), nullptr, srv.GetAddressOf() );


	//float color[ 4 ] = { 0 };
	//pDeviceContext->OMSetRenderTargets( 1, rtv.GetAddressOf(), nullptr );
	//pDeviceContext->IASetInputLayout( nullptr );
	////pDeviceContext->IASetVertexBuffers( 0, 0, nullptr, nullptr, nullptr );
	////pDeviceContext->IASetIndexBuffer( nullptr, DXGI_FORMAT_R32_UINT, NULL );
	//pDeviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	//pDeviceContext->ClearRenderTargetView( rtv.Get(), color );
	//pDeviceContext->Draw( 3, 0 );

	//ID3D11Buffer* pVertexBuffer;

	//struct SimpleVertexCombined
	//{
	//	XMFLOAT3 Pos;
	//	XMFLOAT3 Col;
	//};

	//SimpleVertexCombined verticesCombo[] =
	//{
	//XMFLOAT3( 0.0f, 0.5f, 0.5f ),
	//XMFLOAT3( 0.0f, 0.0f, 0.5f ),
	//XMFLOAT3( 0.5f, -0.5f, 0.5f ),
	//XMFLOAT3( 0.5f, 0.0f, 0.0f ),
	//XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	//XMFLOAT3( 0.0f, 0.5f, 0.0f ),
	//};

	//D3D11_BUFFER_DESC bufferDesc;
	//ZeroMemory( &bufferDesc, sizeof( bufferDesc ) );
	//bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//bufferDesc.ByteWidth = sizeof( SimpleVertexCombined ) * 3;
	//bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//bufferDesc.CPUAccessFlags = 0;
	//bufferDesc.MiscFlags = 0;

	//D3D11_SUBRESOURCE_DATA initData;
	//ZeroMemory( &initData, sizeof( initData ) );
	//initData.pSysMem = verticesCombo;
	//initData.SysMemPitch = 0;
	//initData.SysMemSlicePitch = 0;


	//hr = pDevice->CreateBuffer( &bufferDesc, &initData, &pVertexBuffer );



	//WPARAM wParam = app.MainLoop();

	return 0;
}
