#pragma once
#include <windows.h>
#include <string>
class moving_window
{
private:
	bool register_class(); //funkcja rejestrujaca klase okno - nazwa okna, obsluga komunikatow, kursor, tlo itd
	static std::wstring const s_class_name; //nazwa klasy okna
	static LRESULT CALLBACK window_proc_static( //statyczna funkcja obslugi komunikatow okna
		HWND window, UINT message, //uchwyt okna, komunikat
		WPARAM wparam, LPARAM lparam);
	LRESULT window_proc( //normlana metoda obiektu ktora obsluguje komunikaty
		HWND window, UINT message,
		WPARAM wparam, LPARAM lparam);
	//window_proc_static odbiera komunikat znajduje obiekt moving_window i wywoluje na nim window_proc
	HWND create_window(); 
	HINSTANCE m_instance; //uchyt do okna
	HWND m_main; //uchwyt do glownego okna aplikacji
public:
	moving_window(HINSTANCE instance);
	int run(int show_command);
};