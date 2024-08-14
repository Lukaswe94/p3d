#include "Renderer.h"
#include "FileInfo.h"


Renderer::~Renderer()
{
	device_ptr->Release();
	swap_chain_ptr->Release();
	device_context_ptr->Release();
}

HRESULT Renderer::CreateShaders()
{
	FileInfo vsInfo( ".\\x64\\Debug\\VertexShader.cso" );
	FileInfo psInfo( ".\\x64\\Debug\\PixelShader.cso" );
	HRESULT vhr = device_ptr->CreateVertexShader( vsInfo.GetBytes(), vsInfo.GetSize(), nullptr, vertex_shader_ptr.GetAddressOf() );
	HRESULT phr = device_ptr->CreatePixelShader( psInfo.GetBytes(), psInfo.GetSize(), nullptr, pixel_shader_ptr.GetAddressOf() );
	return vhr & phr;
}

HRESULT Renderer::Initialize( Window wnd )
{
	HRESULT hr;
	RECT rect = wnd.GetWindowRect();
	long width = rect.right - rect.left;
	long height = rect.bottom - rect.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory( &swapChainDesc, sizeof( DXGI_SWAP_CHAIN_DESC ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = wnd.GetHandle();
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

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
		&swap_chain_ptr,
		&device_ptr,
		&featureLvl,
		&device_context_ptr
	);
	if ( FAILED( hr ) )
	{
		return hr;
	}
	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
	hr = swap_chain_ptr->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&back_buffer );
	if ( FAILED( hr ) )
	{
		return hr;
	}
	hr = device_ptr->CreateRenderTargetView( back_buffer.Get(), nullptr, &render_target_view_ptr );
	if ( FAILED( hr ) )
	{
		return hr;
	}

	D3D11_VIEWPORT viewport = {0.0f, 0.0f, width, height, 0.1f, 0.1f};
	device_context_ptr->RSSetViewports( 1, &viewport );
}

void Renderer::Render()
{
	device_context_ptr->OMSetRenderTargets( 1, &render_target_view_ptr, nullptr );
	device_context_ptr->IASetInputLayout( nullptr );
	device_context_ptr->IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	device_context_ptr->VSSetShader( vertex_shader_ptr.Get(), nullptr, 0);
	device_context_ptr->PSSetShader( pixel_shader_ptr.Get(), nullptr, 0 );

	float color[ 4 ] = { 0 };
	device_context_ptr->ClearRenderTargetView( render_target_view_ptr, color );
	device_context_ptr->Draw( 3, 0 );
}

void Renderer::Update()
{
}
