#pragma once
#include <windows.h>
#include <string>
#include <array>
class moving_window
{
private:
	bool register_class(); 
	static std::wstring const s_class_name; 
	static LRESULT CALLBACK window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	LRESULT window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
	HWND create_window(); 
	void create_square(); //funkcja tworzaca kwadrat w srodku oknie 
	void generate_colors();
	void update_square_color();

	HINSTANCE m_instance; 
	HWND m_main; 
	HWND m_square; //uchwyt do okna malego kwadratu 100x100
	HBRUSH m_square_brush; //brush kwadratu
	HBRUSH m_field_brush;
	//pozycje kwadratu 
	int m_square_x; 
	int m_square_y;
	//o ile sie przesuwamy
	int m_dx;
	int m_dy;

	std::array<COLORREF, 16> m_colors;
	int m_color_index;
public:
	moving_window(HINSTANCE instance);
	int run(int show_command);
};