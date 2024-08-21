#pragma once

#include "P3dWin.h"
#include <d3d11.h>
#include <ostream>
#include "Window.h"
#include <wrl.h>

class Renderer
{
	public:
	Renderer() = delete;
	Renderer( Window& wnd );
	Renderer( Renderer&& renderer ) noexcept;
	~Renderer();

	void CreateShaders();
	//depthview
	void CreateViews();
	void CreateViewport();
	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateSwapchainAndDevice();
	void Render();
	void Update();

	private: 
	Window window;
	D3D11_VIEWPORT _viewport;
	Microsoft::WRL::ComPtr<ID3D11Device> _device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> _swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> _renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> _depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> _rasterState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> _depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11Debug> _debug;
};

