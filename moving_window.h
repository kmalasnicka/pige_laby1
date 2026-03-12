#pragma once
#include <windows.h>
#include <string>
class moving_window
{
private:
	bool register_class(); 
	static std::wstring const s_class_name; 
	static LRESULT CALLBACK window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	HWND create_window(); 
	void create_square(); //funkcja tworzaca kwadrat w srodku oknie 
	HINSTANCE m_instance; 
	HWND m_main; 
	HWND m_square; //uchwyt do okna malego kwadratu 100x100
	HBRUSH m_square_brush; //brush kwadratu
	HBRUSH m_field_brush;
public:
	moving_window(HINSTANCE instance);
	int run(int show_command);
};