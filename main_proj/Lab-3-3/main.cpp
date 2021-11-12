/*----------main.cpp----------*/
#include <Windows.h>
#include <Process.h>

// Главная функция программы
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Использование события
	HANDLE ev = CreateEvent(NULL, TRUE, FALSE, L"LabEventSync");
	MessageBox(NULL, L"После нажатия кнопки активируется событие", L"Синхронизация. События", MB_OK);
	SetEvent(ev);
	MessageBox(NULL, L"После нажатия кнопки дескриптор события будет закрыт, а программа завершена", L"Синхронизация. События", MB_OK);
	ResetEvent(ev);
	CloseHandle(ev);
	return NULL;
}