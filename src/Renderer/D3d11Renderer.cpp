#include "D3d11Renderer.h"
#include "../App.h"

using namespace Microsoft::WRL;

D3d11Renderer::D3d11Renderer( std::shared_ptr<Window> wnd ) : window( std::move( wnd ) )
{
	CreateSwapchainAndDevice();
	CreateShaders();
	CreateViews();
	InitViewport();
	CreateDepthStencilState();
	CreateRasterizerState();
	Triangle t( 1, 1, DirectX::XMFLOAT3(0, 0, 0));
	CreateBuffers( t );
	CreateInputLayout();

	DX::ThrowIfFailed(
		device->QueryInterface<ID3D11Debug>( &debug )

	);
}

D3d11Renderer::D3d11Renderer( D3d11Renderer&& renderer ) noexcept :
	window( std::move( renderer.window ) ),
	viewport( std::move( renderer.viewport ) ),
	vsBlob( std::move( renderer.vsBlob ) ),
	device( std::move( renderer.device ) ),
	swapChain( std::move( renderer.swapChain ) ),
	deviceContext( std::move( renderer.deviceContext ) ),
	vertexShader( std::move( renderer.vertexShader ) ),
	pixelShader( std::move( renderer.pixelShader ) ),
	renderTargetView( std::move( renderer.renderTargetView ) ),
	rasterState( std::move( renderer.rasterState ) ),
	depthStencilState( std::move( renderer.depthStencilState ) ),
	vertexBuffer( std::move( renderer.vertexBuffer ) ),
	inputLayout( std::move( renderer.inputLayout ) )
{

}


Microsoft::WRL::ComPtr<ID3D11Debug>& D3d11Renderer::GetDebugInterface()
{
	return debug;
}

void D3d11Renderer::CreateShaders()
{
	FileInfo vsInfo( R"(.\x64\Debug\VertexShader.cso)" );
	FileInfo psInfo( R"(.\x64\Debug\PixelShader.cso)" );

	vsBlob = std::make_unique<FileInfo>( vsInfo );
	psBlob = std::make_unique<FileInfo>( psInfo );

	DX::ThrowIfFailed(
		device->CreateVertexShader(
		vsInfo.getBytes(),
		vsInfo.getSize(),
		nullptr,
		vertexShader.ReleaseAndGetAddressOf() )
	);

	DX::ThrowIfFailed(
		device->CreatePixelShader(
		psInfo.getBytes(),
		psInfo.getSize(),
		nullptr,
		pixelShader.ReleaseAndGetAddressOf() )
	);
}

void D3d11Renderer::CreateViews()
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;

	DX::ThrowIfFailed( swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ),
		reinterpret_cast< LPVOID* >( backBuffer.GetAddressOf() ) ) );

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory( &rtvDesc, sizeof( rtvDesc ) );
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;


	DX::ThrowIfFailed( device->CreateRenderTargetView(
		backBuffer.Get(),
		&rtvDesc,
		renderTargetView.GetAddressOf() )
	);
}

void D3d11Renderer::InitViewport()
{
	const long width = window->GetWidth();
	const long height = window->GetHeight();
	viewport.Width = static_cast< FLOAT >( width );
	viewport.Height = static_cast< FLOAT >( height );
	viewport.TopLeftX = D3D11_DEFAULT_VIEWPORT_TOPLEFTX;
	viewport.TopLeftY = D3D11_DEFAULT_VIEWPORT_TOPLEFTY;
	viewport.MinDepth = D3D11_DEFAULT_VIEWPORT_MIN_DEPTH;
	viewport.MaxDepth = 1;
}

void D3d11Renderer::CreateDepthStencilState()
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

	DX::ThrowIfFailed( device->CreateDepthStencilState(
		&depthStencilDesc,
		depthStencilState.GetAddressOf() ) );
	deviceContext->OMSetDepthStencilState( depthStencilState.Get(), 1 );
}

void D3d11Renderer::CreateRasterizerState()
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

	DX::ThrowIfFailed( device->CreateRasterizerState( &rasterDesc, rasterState.GetAddressOf() ) );
	deviceContext->RSSetState( rasterState.Get() );
}

void D3d11Renderer::CreateSwapchainAndDevice()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
	swapChainDesc.BufferDesc.Width = window->GetWidth();
	swapChainDesc.BufferDesc.Height = window->GetHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = window->GetHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;


	constexpr D3D_FEATURE_LEVEL featureLevels[ 1 ] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL featureLvl;
	DX::ThrowIfFailed(
		D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevels,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		swapChain.ReleaseAndGetAddressOf(),
		device.ReleaseAndGetAddressOf(),
		&featureLvl,
		deviceContext.ReleaseAndGetAddressOf()
	) );
}

void D3d11Renderer::CreateBuffers( const Shape& shape )
{
	D3D11_BUFFER_DESC bufferDesc = {};
	ZeroMemory( &bufferDesc, sizeof( D3D11_BUFFER_DESC ) );
	bufferDesc.ByteWidth = App::GetVertexBufferSize();
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.StructureByteStride = sizeof( vertex );

	D3D11_SUBRESOURCE_DATA vertexData;
	ZeroMemory( &vertexData, sizeof( D3D11_SUBRESOURCE_DATA ) );
	vertexData.pSysMem = shape.GetVertices();
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	DX::ThrowIfFailed(
		device->CreateBuffer(
		&bufferDesc,
		&vertexData,
		vertexBuffer.ReleaseAndGetAddressOf()
	) );

	D3D11_BUFFER_DESC stagingBufferDesc = { 0 };
	stagingBufferDesc.Usage = D3D11_USAGE_STAGING;
	stagingBufferDesc.ByteWidth = App::GetStagingBufferSize();
	stagingBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	stagingBufferDesc.BindFlags = 0;

	DX::ThrowIfFailed(
		device->CreateBuffer(
		&stagingBufferDesc,
		nullptr,
		stagingBuffer.ReleaseAndGetAddressOf()
	) );
}

void D3d11Renderer::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*{ "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		*/
	};
	DX::ThrowIfFailed(
		device->CreateInputLayout(
		inputElementDesc,
		ARRAYSIZE( inputElementDesc ),
		vsBlob->getBytes(),
		vsBlob->getSize(),
		inputLayout.ReleaseAndGetAddressOf()
	) );
}

void D3d11Renderer::Render()
{
	constexpr float color[ 4 ] = { 1.0f, 0.0f,0.0f, 0.2f };
	deviceContext->OMSetRenderTargets( 1, renderTargetView.GetAddressOf(), nullptr );
	deviceContext->RSSetViewports( 1, &viewport );
	deviceContext->ClearRenderTargetView( renderTargetView.Get(), color );

	constexpr UINT offset = 0;
	constexpr UINT size = sizeof( vertex );

	/*memcpy(stagingBuffer->)*/

	//ComPtr<D3D11_MAPPED_SUBRESOURCE> mappedSubresource;
	//
	//DX::ThrowIfFailed(
	//	deviceContext->Map( stagingBuffer.Get(), 0, D3D11_MAP_READ, 0, mappedSubresource.Get() )
	//);
	deviceContext->IASetVertexBuffers( 0, 1, vertexBuffer.GetAddressOf(), &size, &offset );
	deviceContext->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	deviceContext->IASetInputLayout( inputLayout.Get() );

	deviceContext->VSSetShader( vertexShader.Get(), nullptr, 0 );
	deviceContext->PSSetShader( pixelShader.Get(), nullptr, 0 );

#if _DEBUG
	DX::ThrowIfFailed(
		debug->ValidateContext( deviceContext.Get() )
	);
#endif

	deviceContext->Draw( 3, 0 );

	DX::ThrowIfFailed(
		swapChain->Present( 1, 0 )
	);
}

void D3d11Renderer::Update()
{

}
