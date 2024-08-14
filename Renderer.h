#pragma once

#include "P3dWin.h"
#include <d3d11.h>
#include <ostream>
#include "Window.h"
#include <wrl.h>

class Renderer
{
	public:
	Renderer() = default;
	bool operator==( const Renderer& other ) const = default;
	~Renderer();

	HRESULT CreateShaders();
	HRESULT Initialize( Window wnd );
	void Render();
	void Update();

	private: 
	ID3D11Device* device_ptr;
	IDXGISwapChain* swap_chain_ptr;
	ID3D11DeviceContext* device_context_ptr;
	ID3D11RenderTargetView* render_target_view_ptr;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader_ptr;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader_ptr;
};

