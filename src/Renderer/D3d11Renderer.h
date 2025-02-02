#pragma once
#include "../FileInfo.h"
#include "../Window.h"
#include <d3d11.h>
#include <d3d11sdklayers.h>
#include <wrl.h>
#include "../Geometry/Triangle.h"
#include "Renderer.h"
#include "../Geometry/Shape.h"


class D3d11Renderer : public Renderer
{
	public:
	D3d11Renderer() = delete;
	explicit D3d11Renderer( std::shared_ptr<Window> wnd );
	D3d11Renderer( D3d11Renderer&& renderer ) noexcept;
	~D3d11Renderer() override = default;

	[[nodiscard]]Microsoft::WRL::ComPtr<ID3D11Debug>& GetDebugInterface();
	void CreateShaders();
	void CreateViews();
	void InitViewport();
	void CreateDepthStencilState();
	void CreateRasterizerState();
	void CreateSwapchainAndDevice();
	void CreateBuffers(const Shape& shape );
	void CreateInputLayout();

	void Render() override;
	void Update() override;



	private: 
	std::shared_ptr<Window> window;
	D3D11_VIEWPORT viewport;

	std::unique_ptr<FileInfo> vsBlob;
	std::unique_ptr<FileInfo> psBlob;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> stagingBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	Microsoft::WRL::ComPtr<ID3D11Debug> debug;
};

