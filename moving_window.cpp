#include "moving_window.h"
#include <stdexcept>

std::wstring const moving_window::s_class_name{ L"window" };

bool moving_window::register_class() {
	WNDCLASSEXW desc{};
	if (GetClassInfoExW(m_instance, s_class_name.c_str(), &desc) != 0) return true;
	desc = { 
	.cbSize = sizeof(WNDCLASSEXW),
	.lpfnWndProc = window_proc_static,
	.hInstance = m_instance,
	.hCursor = LoadCursorW(nullptr, IDC_ARROW),
	.lpszClassName = s_class_name.c_str() 
	};
	return RegisterClassExW(&desc) != 0;
}

HWND moving_window::create_window()
{
	return CreateWindowExW(
		0,
		s_class_name.c_str(),
		L"window",
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION |
		WS_BORDER | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 
		CW_USEDEFAULT, 0, 
		nullptr,
		nullptr,
		m_instance,
		this);
}

LRESULT moving_window::window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam){
	moving_window* app = nullptr;
	if (message == WM_NCCREATE)
	{
		auto p = reinterpret_cast<LPCREATESTRUCTW>(lparam);
		app = static_cast<moving_window*>(p->lpCreateParams);
		SetWindowLongPtrW(window, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(app));
	}
	else
	{
		app = reinterpret_cast<moving_window*>(
			GetWindowLongPtrW(window, GWLP_USERDATA));
	}
	if (app != nullptr)
	{
		return app->window_proc(window, message,
			wparam, lparam);
	}
	return DefWindowProcW(window, message, wparam, lparam);
}

LRESULT moving_window::window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam){
	switch (message) {
	case WM_CLOSE:
		DestroyWindow(window);
		return 0;
	case WM_DESTROY:
		if (window == m_main)
			PostQuitMessage(EXIT_SUCCESS);
		return 0;
	}
	return DefWindowProcW(window, message, wparam, lparam);
}

moving_window::moving_window(HINSTANCE instance) : m_instance{ instance }, m_main{}{
	register_class();
	m_main = create_window();
}

int moving_window::run(int show_command)
{
	ShowWindow(m_main, show_command);
	MSG msg{};
	BOOL result = TRUE;
	while ((result = GetMessageW(&msg, nullptr, 0, 0)) != 0)
	{
		if (result == -1) return EXIT_FAILURE;
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return EXIT_SUCCESS;
}