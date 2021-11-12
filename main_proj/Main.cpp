/*----------Main.cpp----------*/
#include <Windows.h>
#include "CreateWindowEasy.hpp"

// Главная функция программы
int WINAPI WinMain(
	HINSTANCE hInstance,		// дескриптор экземпляра приложени¤
	HINSTANCE hPrevInstance,	// в Win32 не используется
	LPSTR lpCmdLine,			// нужен для запуска окна в режиме командной строки
	int nCmdShow				// режим отображения кона
) {
	HWND window = MainWindow::CreateWindowEasy(hInstance, nCmdShow);
	return NULL;
}