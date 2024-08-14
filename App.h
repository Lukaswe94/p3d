#pragma once
#include "P3dWin.h" 
#include "Window.h"
#include "Renderer.h"

class App
{
	public:
	App( Window&window, Renderer &renderer ) noexcept;
	~App();
	WPARAM MainLoop();

	private:
	Window wnd;
	Renderer renderer;
};

