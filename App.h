#pragma once
#include "P3dWin.h" 
#include "Window.h"
#include "Renderer.h"

class App
{
	public:
	App() = delete;
	App( Window& window, Renderer& renderer ) noexcept;
	~App();
	WPARAM MainLoop();

	private:
	Window window;
	Renderer renderer;
};

