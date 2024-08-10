#pragma once
#include "P3dWin.h" 
#include "Win32Window.h"

class Win32App
{
	public:
	Win32App( Win32Window &window ) noexcept;
	~Win32App();
	WPARAM MainLoop();

	private:
	Win32Window wnd;
};

