#include <fstream>
#include <random>
#include <Windows.h>
#include "Mailslot.hpp"
#include "Pipe.hpp"
#include "FileMap.hpp"

LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
int GetRandNum(int, int);
void GenerateFile(UINT, int, int);

// Идентификаторы кнопок внутри окна
const BYTE ID_BTN_LAB4_1 = 1;
const BYTE ID_BTN_LAB4_2 = 2;
const BYTE ID_BTN_LAB4_3 = 3;

// Главная функция программы
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// Создаём и регистрируем класс главного окна
	WNDCLASS mainWinClass = { 0 };
	mainWinClass.style = 0;
	mainWinClass.lpfnWndProc = MainWinProc;
	mainWinClass.cbClsExtra = mainWinClass.cbWndExtra = 0;
	mainWinClass.hInstance = hInstance;
	mainWinClass.hIcon = NULL;
	mainWinClass.hCursor = NULL;
	mainWinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	mainWinClass.lpszMenuName = NULL;
	mainWinClass.lpszClassName = L"MainWindowClass";
	if (!RegisterClass(&mainWinClass)) return FALSE;

	// Создаём главное окно и отображаем его
	DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
	HWND hMainWnd = CreateWindow(L"MainWindowClass", L"Ћаб. работа є4", wndStyle,
		CW_USEDEFAULT, CW_USEDEFAULT, 250, 250, NULL, NULL, hInstance, NULL);
	if (!hMainWnd) return FALSE;
	ShowWindow(hMainWnd, nCmdShow);
	UpdateWindow(hMainWnd);

	// Цикл обработки событий окна
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return NULL;
}

// Функция обработки сообщений для главного окна
LRESULT CALLBACK MainWinProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
	WORD hiWord, loWord;
	switch (msg) {
	// При создании окна создаём в нём кнопки
	case WM_CREATE:
		CreateWindow(L"button", L"Mailslot", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			15, 15, 200, 50, hw, (HMENU)ID_BTN_LAB4_1, NULL, NULL);
		CreateWindow(L"button", L"Pipe", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			15, 80, 200, 50, hw, (HMENU)ID_BTN_LAB4_2, NULL, NULL);
		CreateWindow(L"button", L"File Mapping", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
			15, 145, 200, 50, hw, (HMENU)ID_BTN_LAB4_3, NULL, NULL);
		return 0;

	// Обработчики нажатия кнопок
	case WM_COMMAND:
		hiWord = HIWORD(wp); loWord = LOWORD(wp);
		if ((hiWord == 0) && (loWord == ID_BTN_LAB4_1)) {
			const UINT array_size = 15;
			GenerateFile(array_size, -10, 10);
			Mailslot::MailslotMain();

		}
		else if ((hiWord == 0) && (loWord == ID_BTN_LAB4_2)) {
			const UINT array_size = 15;
			GenerateFile(array_size, -10, 10);
			Pipe::PipeMain();
		}
		else if ((hiWord == 0) && (loWord == ID_BTN_LAB4_3)) {
			const UINT array_size = 15;
			GenerateFile(array_size, -10, 10);
			FileMap::FileMapMain();
		}
		return 0;

	// Обработка закрытия окна
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hw, msg, wp, lp);
}

// Функция получения случайного числа в заданном диапазоне
int GetRandNum(int min, int max) {
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(min, max);
	int res = distribution(generator);
	return res;
}

// Сгенерировать случайный файл
void GenerateFile(UINT nums, int start, int end) {
	std::wofstream fileWrite;
	std::locale defaultLocale("");
	fileWrite.imbue(defaultLocale);
	fileWrite.open("text.txt");
	for (UINT i = 0; i < nums; i++) {
		fileWrite << GetRandNum(start, end) << L' ';
	}
}