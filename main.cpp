#include <windows.h>
#include "moving_window.h"

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE prevInstance, LPWSTR command_line, int show_command){
	moving_window app{ instance };
	return app.run(show_command);
}