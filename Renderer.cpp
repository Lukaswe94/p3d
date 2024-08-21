#include "Renderer.h"
#include "FileInfo.h"
#include <cassert>


Renderer::Renderer( Window& wnd ) : window( std::move( wnd ) )
{
	CreateSwapchainAndDevice();
	CreateShaders();
	CreateViews();
	CreateViewport();
	CreateDepthStencilState();
	CreateRasterizerState();
}

Renderer::Renderer( Renderer&& renderer ) noexcept :
	window( std::move( renderer.window ) ),
	_viewport( renderer._viewport )
{
	_device.Attach( renderer._device.Get() );
	_deviceContext.Attach( renderer._deviceContext.Get() );
	_swapChain.Attach( renderer._swapChain.Get() );
	_pixelShader.Attach( renderer._pixelShader.Get() );
	_vertexShader.Attach( renderer._vertexShader.Get() );
	_depthStencilState.Attach( renderer._depthStencilState.Get() );
	_rasterState.Attach( renderer._rasterState.Get() );
	_renderTargetView.Attach( renderer._renderTargetView.Get() );
	renderer._device.Detach();
	renderer._deviceContext.Detach();
	renderer._swapChain.Detach();
	renderer._pixelShader.Detach();
	renderer._vertexShader.Detach();
	renderer._depthStencilState.Detach();
	renderer._rasterState.Detach();
	renderer._renderTargetView.Detach();
}

Renderer::~Renderer()
{
}

void Renderer::CreateShaders()
{
	FileInfo vsInfo( ".\\x64\\Debug\\VertexShader.cso" );
	FileInfo psInfo( ".\\x64\\Debug\\PixelShader.cso" );

	ThrowIfFailed( _device->CreateVertexShader(
		vsInfo.GetBytes(),
		vsInfo.GetSize(),
		nullptr,
		_vertexShader.GetAddressOf() ) );

	ThrowIfFailed( _device->CreatePixelShader(
		psInfo.GetBytes(),
		psInfo.GetSize(),
		nullptr,
		_pixelShader.GetAddressOf() ) );
}

void Renderer::CreateViews()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;

	ThrowIfFailed( _swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),
		( LPVOID* )back_buffer.GetAddressOf() ) );

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	ZeroMemory( &rtv_desc, sizeof( rtv_desc ) );
	rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;


	ThrowIfFailed( _device->CreateRenderTargetView(
		back_buffer.Get(),
		&rtv_desc,
		_renderTargetView.GetAddressOf() ) );
}

void Renderer::CreateViewport()
{
	long width = window.GetWidth();
	long height = window.GetHeight();
	_viewport.Width = ( FLOAT )width;
	_viewport.Height = ( FLOAT )height;
	_viewport.TopLeftX = D3D11_DEFAULT_VIEWPORT_TOPLEFTX;
	_viewport.TopLeftY = D3D11_DEFAULT_VIEWPORT_TOPLEFTY;
	_viewport.MinDepth = D3D11_DEFAULT_VIEWPORT_MIN_DEPTH;
	_viewport.MaxDepth = 1;
}

void Renderer::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory( &depthStencilDesc, sizeof( depthStencilDesc ) );
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	ThrowIfFailed( _device->CreateDepthStencilState( &depthStencilDesc,
		_depthStencilState.GetAddressOf() ) );
	_deviceContext->OMSetDepthStencilState( _depthStencilState.Get(), 1 );
}

void Renderer::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC rasterDesc = {};
	ZeroMemory( &rasterDesc, sizeof( rasterDesc ) );
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	ThrowIfFailed( _device->CreateRasterizerState( &rasterDesc, _rasterState.GetAddressOf() ) );
	_deviceContext->RSSetState( _rasterState.Get() );
}

void Renderer::CreateSwapchainAndDevice()
{
	HRESULT hr;
	long width = window.GetWidth();
	long height = window.GetHeight();

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window.GetHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;


	D3D_FEATURE_LEVEL featureLevels[ 1 ] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL featureLvl;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		_swapChain.GetAddressOf(),
		_device.GetAddressOf(),
		&featureLvl,
		_deviceContext.GetAddressOf()
	);
	ThrowIfFailed( hr );
}
//
//void Renderer::Initialize( Window& wnd )
//{
//	HRESULT hr;
//	RECT rect = wnd.GetWindowRect();
//	long width = rect.right - rect.left;
//	long height = rect.bottom - rect.top;
//
//	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
//	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
//	swapChainDesc.BufferDesc.Width = width;
//	swapChainDesc.BufferDesc.Height = height;
//	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
//	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
//	swapChainDesc.SampleDesc.Count = 1;
//	swapChainDesc.SampleDesc.Quality = 0;
//	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
//	swapChainDesc.BufferCount = 1;
//	swapChainDesc.OutputWindow = wnd.GetHandle();
//	swapChainDesc.Windowed = true;
//	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
//	swapChainDesc.Flags = 0;
//
//
//	D3D_FEATURE_LEVEL featureLevels[ 1 ] = { D3D_FEATURE_LEVEL_11_0 };
//	D3D_FEATURE_LEVEL featureLvl;
//
//	hr = D3D11CreateDeviceAndSwapChain(
//		nullptr,
//		D3D_DRIVER_TYPE_HARDWARE,
//		nullptr,
//		D3D11_CREATE_DEVICE_DEBUG,
//		featureLevels,
//		1,
//		D3D11_SDK_VERSION,
//		&swapChainDesc,
//		&swap_chain_ptr,
//		&device_ptr,
//		&featureLvl,
//		&device_context_ptr
//	);
//	if ( FAILED( hr ) )
//	{
//		throw;
//	}
//
//	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
//	hr = swap_chain_ptr->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )back_buffer.GetAddressOf() );
//	if ( FAILED( hr ) )
//	{
//		throw;
//	}
//	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
//	ZeroMemory( &rtv_desc, sizeof( rtv_desc ) );
//	rtv_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//	rtv_desc.Texture2D.MipSlice = 0;
//
//	hr = device_ptr->CreateRenderTargetView( back_buffer.Get(), &rtv_desc, _renderTargetView.GetAddressOf() );
//	back_buffer->Release();
//	if ( FAILED( hr ) )
//	{
//		throw;
//	}
//
//	_viewport->Width = ( FLOAT )width;
//	_viewport->Height = ( FLOAT )height;
//	_viewport->TopLeftX = D3D11_DEFAULT_VIEWPORT_TOPLEFTX;
//	_viewport->TopLeftY = D3D11_DEFAULT_VIEWPORT_TOPLEFTY;
//	_viewport->MinDepth = D3D11_DEFAULT_VIEWPORT_MIN_DEPTH;
//	_viewport->MaxDepth = 1;
//	device_context_ptr->RSSetViewports( 1, _viewport.Get() );
//
//	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
//	ZeroMemory( &depth_stencil_desc, sizeof( depth_stencil_desc ) );
//	depth_stencil_desc.DepthEnable = true;
//	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
//	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
//	depth_stencil_desc.StencilEnable = true;
//	depth_stencil_desc.StencilReadMask = 0xFF;
//	depth_stencil_desc.StencilWriteMask = 0xFF;
//
//}

void Renderer::Render()
{
	float color[ 4 ] = { 1.0f, 0.0f,0.0f, 0.2f };
	_deviceContext->OMSetRenderTargets( 1, _renderTargetView.GetAddressOf(), NULL );

	_deviceContext->RSSetViewports( 1, &_viewport );

	_deviceContext->ClearRenderTargetView( _renderTargetView.Get(), color );

	_deviceContext->IASetInputLayout( NULL );
	_deviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	_deviceContext->VSSetShader( _vertexShader.Get(), NULL, 0 );
	_deviceContext->PSSetShader( _pixelShader.Get(), NULL, 0 );
	_deviceContext->Draw( 3, 0 );

	ThrowIfFailed( _swapChain->Present( 0, 0 ) );
}

void Renderer::Update()
{
}
