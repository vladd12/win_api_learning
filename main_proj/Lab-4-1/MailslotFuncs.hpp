#ifndef MAILSLOTFUNCS
#define MAILSLOTFUNCS

#include <string>
#include <sstream>
#include <windows.h>
#include <process.h>
#include <strsafe.h>
#include <tchar.h>
#include <iostream>

HANDLE WINAPI MakeMailslot(LPCWSTR);
void WINAPI WriteClientSlot(LPCWSTR, double*);
BOOL WINAPI ReadServerSlot(HANDLE, double*);

// Создание сервера почтового ящика (Mailslot)
HANDLE WINAPI MakeMailslot(LPCWSTR SlotName) {
	// Создание дескриптора почтового ящика
	HANDLE hSlot = CreateMailslot(SlotName, 0, MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);
	if (hSlot == INVALID_HANDLE_VALUE) {
		std::wstring data = L"CreateMailslot failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return NULL;
	}
	else return hSlot;
}

// Запись в клиентский Mailslot
void WINAPI WriteClientSlot(LPCWSTR SlotName, double* Message) {
	// Создание файла
	HANDLE hFile = CreateFile(SlotName, GENERIC_WRITE, FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	// Проверка на ошибку при создании
	if (hFile == INVALID_HANDLE_VALUE) {
		std::wstring data = L"CreateFile failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return;
	}

	// Запись в Mailslot
	DWORD cbWritten;
	BOOL fResult = WriteFile(hFile, Message, sizeof(double), &cbWritten, (LPOVERLAPPED)NULL);
	// Проверка на ошибку при записи
	if (!fResult) {
		std::wstring data = L"WriteFile failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return;
	}
	CloseHandle(hFile);
}

// Чтение из серверного Mailslot
BOOL WINAPI ReadServerSlot(HANDLE hSlot, double* inBuffer) {
	// Читаем информацию о сообщения
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ServerReadSlot"));
	DWORD cbMessage = 0, cMessage = 0, cbRead = 0;
	BOOL fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);

	// Если сообщений нет
	if (cbMessage == MAILSLOT_NO_MESSAGE) {
		printf("Waiting for a message...\n");
		return TRUE;
	}
	// Если сообщения есть
	else {
		DWORD cAllMessages = cMessage;
		while (cMessage != 0) {
			OVERLAPPED ov;
			ov.Offset = 0;
			ov.OffsetHigh = 0;
			ov.hEvent = hEvent;
			fResult = ReadFile(hSlot, inBuffer, cbMessage, &cbRead, &ov);

			std::wcout << inBuffer[0] << L' ' << inBuffer[1] << L'\n'
				<< inBuffer[2] << L' ' << inBuffer[3] << L'\n';
			fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);
		}
	}
	CloseHandle(hEvent);
	return TRUE;
}

#endif