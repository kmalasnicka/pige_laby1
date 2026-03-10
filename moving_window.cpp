#include "moving_window.h"
#include <stdexcept>

std::wstring const moving_window::s_class_name{ L"window" }; //nazwa okna

bool moving_window::register_class() {
	WNDCLASSEXW desc{}; // struktura opisujaca klase okna
	if (GetClassInfoExW(m_instance, s_class_name.c_str(), &desc) != 0) return true; //jesli juz istnieje 
	desc = { 
	.cbSize = sizeof(WNDCLASSEXW), //rozmiar struktury
	.lpfnWndProc = window_proc_static,
	.hInstance = m_instance,
	.hCursor = LoadCursorW(nullptr, IDC_ARROW), //zwykła strzałka
	.lpszClassName = s_class_name.c_str() //ustawiamy nazwe klasy okna
	};
	return RegisterClassExW(&desc) != 0;
}

HWND moving_window::create_window()
{
	return CreateWindowExW(
		0, //brak rozszerzonych stylow
		s_class_name.c_str(), //nazwa klasy okna
		L"window", //tytul okna
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | //zwykłe okno najwyższego poziomu, ma menu systemowe, ma pasek tytulu
		WS_BORDER | WS_MINIMIZEBOX, //ma ramke, ma przycisk minimalizacji
		//WS_THICKFRAME - mozna rozciagac
		CW_USEDEFAULT, 0, 
		CW_USEDEFAULT, 0, 
		nullptr, //nie ma okna nadrzednego
		nullptr, //nie ma menu
		m_instance,
		this); //wskaznik do obiektu moving_window
}

LRESULT moving_window::window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam){
	moving_window* app = nullptr;
	if (message == WM_NCCREATE) //komunikat podczas tworzenia okna
	{
		auto p = reinterpret_cast<LPCREATESTRUCTW>(lparam);
		app = static_cast<moving_window*>(p->lpCreateParams);
		SetWindowLongPtrW(window, GWLP_USERDATA,
			reinterpret_cast<LONG_PTR>(app));
	}
	else //proba odzyskania wskaznika do obiektu
	{
		app = reinterpret_cast<moving_window*>(GetWindowLongPtrW(window, GWLP_USERDATA));
	}
	if (app != nullptr) //jelsi udalo sie znalezc obiekt to przekazujemy komunikat do zwyklej metody
	{
		return app->window_proc(window, message, wparam, lparam);
	}
	return DefWindowProcW(window, message, wparam, lparam);
}

LRESULT moving_window::window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam){
	switch (message) {
	case WM_CLOSE: //zamkniecie okna
		DestroyWindow(window);
		return 0;
	case WM_DESTROY: //okno zostalo zniszczone
		if (window == m_main)
			PostQuitMessage(EXIT_SUCCESS);
		return 0;
	}//domyslna obsluga windowska
	return DefWindowProcW(window, message, wparam, lparam);
}

moving_window::moving_window(HINSTANCE instance) : m_instance{ instance }, m_main{}{
	register_class();
	m_main = create_window();
}

int moving_window::run(int show_command)
{
	ShowWindow(m_main, show_command); //pokazuje okno na ekaznie
	MSG msg{}; //struktura do komunikatow
	BOOL result = TRUE; //zmienna do przechowywania GetMessageW
	while ((result = GetMessageW(&msg, nullptr, 0, 0)) != 0) 
	{
		if (result == -1) return EXIT_FAILURE; 
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return EXIT_SUCCESS;
}