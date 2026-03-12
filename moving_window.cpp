#include "moving_window.h"
#include <stdexcept>

std::wstring const moving_window::s_class_name{ L"moving_window_class" };

bool moving_window::register_class() {
	WNDCLASSEXW desc{};
	if (GetClassInfoExW(m_instance, s_class_name.c_str(), &desc) != 0) return true;
	desc = {
	.cbSize = sizeof(WNDCLASSEXW),
	.lpfnWndProc = window_proc_static,
	.hInstance = m_instance,
	.hCursor = LoadCursorW(nullptr, IDC_ARROW),
	.hbrBackground = m_field_brush, //przypisujemy pedzel do klasy okna
	.lpszClassName = s_class_name.c_str()
	};
	return RegisterClassExW(&desc) != 0;
}

HWND moving_window::create_window()
{
	RECT rect{ 0, 0, 800, 600 }; //left = 0, top = 0, right = 800, bottom = 600 -> obszar 800x600
	//stage 2: WS_CLIPCHILDREN (nie maluj swojego tła na obszarze, gdzie są child windows), sprawia ze kwadra w srodku jest widoczny!
	DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_BORDER | WS_MINIMIZEBOX | WS_CLIPCHILDREN; //WS_CAPTION okno mozna przesuwac, WS_CAPTION dodaje pask tytulu przez co mozna chwycic okno i przesunac, WS_BORDER ramka, WS_MINIMIZEBOX dodaje przycosl - na pasku tytulu
	AdjustWindowRect(&rect, style, FALSE); //okno ma border i title bar wiec ta funkcja mowi policz jaki musi być rozmiar całego okna, żeby środek miał 800×600
	//przwdziwy rozmiar:
	int width = rect.right - rect.left; //calkowita szerokosc 
	int height = rect.bottom - rect.top; //calkowita wysokosc
	return CreateWindowExW(
		0,
		s_class_name.c_str(),
		L"poor man's snake", //nazwa okna
		style,
		CW_USEDEFAULT, CW_USEDEFAULT, //x, y
		width, height, //szrerokosc, wysokosc
		nullptr,
		nullptr,
		m_instance,
		this);
}

void moving_window::create_square() { //funkcja tworzaca kwadrat
	//rozmiar obszaru klienta głównego okna, czyli wnętrza okna, uwzgledniamy square wzgledem srodka wnetrza a nie wzgledem calej ramki
	RECT client{};
	GetClientRect(m_main, &client);
	int square_size = 100; //bok kwadratu to 100 pikseli 
	//wspolrzedne lewego gornego rogu tak zeby square byl na srodku 
	//zapisujemy globalnie x i y
	m_square_x = (client.right - square_size) / 2; 
	m_square_y = (client.bottom - square_size) / 2;

	m_square = CreateWindowExW(
		0, 
		L"STATIC",
		nullptr,
		WS_CHILD | WS_VISIBLE, //dziecko glownego okna, ma byc widoczne od raazu
		m_square_x, m_square_y,
		square_size, square_size,
		m_main, //parent
		nullptr,
		m_instance,
		nullptr
	);
}

LRESULT moving_window::window_proc_static(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	moving_window* app = nullptr;
	if (message == WM_NCCREATE)
	{
		auto p = reinterpret_cast<LPCREATESTRUCTW>(lparam);
		app = static_cast<moving_window*>(p->lpCreateParams);
		SetWindowLongPtrW(window, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(app));
	}
	else
	{
		app = reinterpret_cast<moving_window*>(GetWindowLongPtrW(window, GWLP_USERDATA));
	}
	if (app != nullptr)
	{
		return app->window_proc(window, message, wparam, lparam);
	}
	return DefWindowProcW(window, message, wparam, lparam);
}

LRESULT moving_window::window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
	case WM_CLOSE:
		DestroyWindow(window);
		return 0;
	case WM_DESTROY:
		KillTimer(m_main, 1);
		if (window == m_main)
			PostQuitMessage(EXIT_SUCCESS);
		return 0; 
	case WM_CTLCOLORSTATIC: //square jest static windows pyta jakim pedzlem go malowac
	{
		HWND control = reinterpret_cast<HWND>(lparam); //uchwyt tej konkretnej kontrolki static ktorej windows chce kolor
		if (control == m_square) return reinterpret_cast<INT_PTR>(m_square_brush); //jesli kontrolka to square to zwracamy brush square, czyli square dostaje swoj kolor
		return reinterpret_cast<INT_PTR>(m_field_brush); //w pozostalych przypadkach brush tla 
	}
	case WM_TIMER:
		m_square_x += m_dx; //przesuwamy o 3 piksele w poziomie ruch w lewo
		m_square_y += m_dy; //nic sie nie dzieje bo m_dy = 0
		SetWindowPos( //przesuwa square na nowe wspolrzedne 
			m_square,
			nullptr,
			m_square_x,
			m_square_y,
			100,
			100,
			SWP_NOZORDER
		);
		return 0;
	}
	return DefWindowProcW(window, message, wparam, lparam);
}
//w konstruktorze pedzel do malowania
moving_window::moving_window(HINSTANCE instance) 
	: m_instance{ instance }, 
	m_main{}, 
	m_square{},
	m_field_brush{ CreateSolidBrush(RGB(50, 60, 70))},
	m_square_brush{ CreateSolidBrush(RGB(220, 40, 40))},
	m_square_x{},
	m_square_y{},
	m_dx{-3},
	m_dy{0}
{
	register_class();
	m_main = create_window();
	create_square();
	SetTimer(m_main, 1, 20, nullptr); //co 20 ms program dostaje sygnal
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