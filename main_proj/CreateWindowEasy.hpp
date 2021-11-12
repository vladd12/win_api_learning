/*----------CreateWindowEasy.hpp----------*/
#ifndef CRT_WND_EASY
#define CRT_WND_EASY

#include <Windows.h>
#include "Lab1.hpp"
#include "Lab2.hpp"
#include "Lab3.hpp"
#include "Lab4.hpp"
#include "Lab5.hpp"

namespace MainWindow {
	LRESULT CALLBACK MainWinProc(HWND, UINT, WPARAM, LPARAM);
	HWND CreateWindowEasy(HINSTANCE hInst, int ss);

	// Идентификаторы кнопок внутри окна
	const BYTE ID_MYBUTTON_LAB1 = 1;
	const BYTE ID_MYBUTTON_LAB2 = 2;
	const BYTE ID_MYBUTTON_LAB3 = 3;
	const BYTE ID_MYBUTTON_LAB4 = 4;
	const BYTE ID_MYBUTTON_LAB5 = 5;

	// Глобальные переменные
	HINSTANCE hinst;
	int cmdMode;

	// Создание главного окна программы
	HWND CreateWindowEasy(HINSTANCE hInst, int ss) {
		cmdMode = ss;
		hinst = hInst;
		// Создаём и регистрируем класс главного окна
		WNDCLASS mainWinClass = { 0 };
		mainWinClass.style = 0;
		mainWinClass.lpfnWndProc = MainWinProc;
		mainWinClass.cbClsExtra = mainWinClass.cbWndExtra = 0;
		mainWinClass.hInstance = hInst;
		mainWinClass.hIcon = NULL;
		mainWinClass.hCursor = NULL;
		mainWinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		mainWinClass.lpszMenuName = NULL;
		mainWinClass.lpszClassName = L"MainWindowClass";
		if (!RegisterClass(&mainWinClass)) return FALSE;

		// Создаём главное окно и отображаем его
		DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		HWND hMainWnd = CreateWindow(L"MainWindowClass", L"Ћабы по ќ—", wndStyle,
			CW_USEDEFAULT, CW_USEDEFAULT, 300, 165, NULL, NULL, hInst, NULL);
		if (!hMainWnd) return FALSE;
		ShowWindow(hMainWnd, ss);
		UpdateWindow(hMainWnd);

		// Цикл обработки событий окна
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return hMainWnd;
	}

	// Функция обработки сообщений дл¤ главного окна
	LRESULT CALLBACK MainWinProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
		WORD hiWord, loWord;

		switch (msg) {
		// При создании окна создаём в нём кнопки
		case WM_CREATE:
			CreateWindow(L"button", L"Ћаб. 1", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				15, 15, 100, 20, hw, (HMENU)ID_MYBUTTON_LAB1, NULL, NULL);
			CreateWindow(L"button", L"Ћаб. 2", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				130, 15, 100, 20, hw, (HMENU)ID_MYBUTTON_LAB2, NULL, NULL);
			CreateWindow(L"button", L"Ћаб. 3", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				15, 50, 100, 20, hw, (HMENU)ID_MYBUTTON_LAB3, NULL, NULL);
			CreateWindow(L"button", L"Ћаб. 4", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				130, 50, 100, 20, hw, (HMENU)ID_MYBUTTON_LAB4, NULL, NULL);
			CreateWindow(L"button", L"Ћаб. 5", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				15, 85, 100, 20, hw, (HMENU)ID_MYBUTTON_LAB5, NULL, NULL);
			return 0;

		// Обработчики нажати¤ кнопок
		case WM_COMMAND:
			hiWord = HIWORD(wp); loWord = LOWORD(wp);
			if ((hiWord == 0) && (loWord == ID_MYBUTTON_LAB1)) lab1::Lab1Exec();
			else if ((hiWord == 0) && (loWord == ID_MYBUTTON_LAB2)) lab2::Lab2Exec(hw);
			else if ((hiWord == 0) && (loWord == ID_MYBUTTON_LAB3)) lab3::Lab3Exec(hw);
			else if ((hiWord == 0) && (loWord == ID_MYBUTTON_LAB4)) lab4::Lab4Exec(hinst, cmdMode);
			else if ((hiWord == 0) && (loWord > ID_MYBUTTON_LAB4)) lab5::Lab5Exec(hw);
			return 0;

		// Обработка закрытия окна
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hw, msg, wp, lp);
	}

}

#endif