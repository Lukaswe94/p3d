#pragma once
#include "Window.h"
#include "Renderer/Renderer.h"

class App
{
	public:
	App( std::shared_ptr<Window> window, std::shared_ptr<Renderer> renderer ) noexcept;
	~App() = default;
	WPARAM MainLoop() const;
	static void SetVertexBufferSize(UINT);
	static UINT GetVertexBufferSize();
	static void SetStagingBufferSize(UINT);
	static UINT GetStagingBufferSize();

	private:
	static UINT vertexBufferSize;
	static UINT stagingBufferSize;
	std::shared_ptr<Window> window;
	std::shared_ptr<Renderer> renderer;
};

