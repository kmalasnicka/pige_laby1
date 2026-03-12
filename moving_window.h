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
	void create_square();
	HINSTANCE m_instance; 
	HWND m_main; 
	HWND m_square;
	//square bedzie sie przesuwac wiec trzeba pamietac gdzie aktualnie jest:
	int m_square_x;
	int m_square_y;
	HBRUSH m_square_brush; 
	HBRUSH m_field_brush;

	//kierunek ruchu:
	int m_dx;
	int m_dy;
public:
	moving_window(HINSTANCE instance);
	int run(int show_command);
};