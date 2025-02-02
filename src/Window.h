#pragma once
#include "P3dWin.h"
#include <format>
class Window
{
	public:
	Window() = delete;
	Window( Window&& wnd ) noexcept;
	Window( HINSTANCE hInstance, LPCWSTR pClassname );
	~Window();
	[[nodiscard]] BOOL __stdcall Show( int nCmdShow ) const;
	[[nodiscard]] LPCWSTR __stdcall GetName() const;
	[[nodiscard]] HWND __stdcall GetHandle() const;
	[[nodiscard]] HINSTANCE __stdcall GetInstance() const;
	[[nodiscard]] RECT __stdcall GetWindowRect() const;
	[[nodiscard]] long __stdcall GetWidth() const;
	[[nodiscard]] long __stdcall GetHeight() const;
	[[nodiscard]] int __stdcall ShowErrorWindow( LPCWSTR text, LPCWSTR caption, UINT type ) const;

	private:
	static LRESULT HandleMsg( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
	HWND hWnd;
	HMENU hMenu;
	LPCWSTR classname;
	HINSTANCE hInstance;
};

