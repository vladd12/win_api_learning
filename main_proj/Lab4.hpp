/*----------Lab4.hpp----------*/
#ifndef LAB4
#define LAB4

#include <windows.h>
#include "Lab4Pipe.hpp"
#include "Lab4Mailslot.hpp"
#include "Lab4FileMapping.hpp"

namespace lab4 {
	// ќбъ¤влени¤ функций
	LRESULT CALLBACK Lab4WinProc(HWND, UINT, WPARAM, LPARAM);
	HWND Lab4Exec(HINSTANCE hInst, int ss);

	// »дентификаторы кнопок внутри окна
	const BYTE ID_BUTTON_MAILSLOT     = 1;
	const BYTE ID_BUTTON_FILE_MAPPING = 2;
	const BYTE ID_BUTTON_PIPE         = 3;

	// √лобальные переменные событий
	HANDLE evClientRead = NULL, evServerRead = NULL, evPipeInit = NULL, evGlobalRead = NULL;
	double globalRes = 0;

	// ‘ункци¤ дл¤ создани¤ окна лабораторной работы
	HWND Lab4Exec(HINSTANCE hInst, int ss) {
		// —оздаЄм и регистрируем класс главного окна
		WNDCLASS lab4WinClass = { 0 };
		lab4WinClass.style = 0;
		lab4WinClass.lpfnWndProc = Lab4WinProc;
		lab4WinClass.cbClsExtra = lab4WinClass.cbWndExtra = 0;
		lab4WinClass.hInstance = hInst;
		lab4WinClass.hIcon = NULL;
		lab4WinClass.hCursor = NULL;
		lab4WinClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		lab4WinClass.lpszMenuName = NULL;
		lab4WinClass.lpszClassName = L"Lab4WindowClass";
		if (!RegisterClass(&lab4WinClass)) return FALSE;

		// —оздаЄм главное окно и отображаем его
		DWORD wndStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		HWND hLab4Wnd = CreateWindow(L"Lab4WindowClass", L"Ћаб. работа є4", wndStyle,
			CW_USEDEFAULT, CW_USEDEFAULT, 380, 100, NULL, NULL, hInst, NULL);
		if (!hLab4Wnd) return FALSE;
		ShowWindow(hLab4Wnd, ss);
		UpdateWindow(hLab4Wnd);

		// ÷икл обработки событий окна
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return hLab4Wnd;
	}

	// ‘ункци¤ обработки сообщений дл¤ окна лабораторной работы
	LRESULT CALLBACK Lab4WinProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
		WORD hiWord, loWord;
		switch (msg) {
		// ѕри создании окна создаЄм в нЄм кнопки
		case WM_CREATE:
			CreateWindow(L"button", L"Mailslots", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				15, 15, 100, 20, hw, (HMENU)ID_BUTTON_MAILSLOT, NULL, NULL);
			CreateWindow(L"button", L"Pipes", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				130, 15, 100, 20, hw, (HMENU)ID_BUTTON_PIPE, NULL, NULL);
			CreateWindow(L"button", L"File Mapping", WS_CHILD | BS_PUSHBUTTON | WS_VISIBLE,
				245, 15, 100, 20, hw, (HMENU)ID_BUTTON_FILE_MAPPING, NULL, NULL);
			return 0; 
		// ќбработчики нажати¤ кнопок
		case WM_COMMAND:
			hiWord = HIWORD(wp); loWord = LOWORD(wp);
			if ((hiWord == 0) && (loWord == ID_BUTTON_MAILSLOT)) {
				lab4_1::MailslotMain(hw);
			}
			else if ((hiWord == 0) && (loWord == ID_BUTTON_PIPE)) {
				lab4_2::PipeMain(hw);
			}
			else if ((hiWord == 0) && (loWord == ID_BUTTON_FILE_MAPPING)) {
				lab4_3::FileMappingMain(hw);
			}
			return 0;

			// ќбработка закрыти¤ окна
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hw, msg, wp, lp);

	}
}

#endif