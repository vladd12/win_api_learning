/*----------MailslotFuncs.hpp----------*/
#ifndef MAILSLOTFUNCS
#define LAB4MAILSLOT

#include <string>
#include <sstream>
#include <windows.h>
#include <process.h>
#include <strsafe.h>
#include <tchar.h>

HANDLE WINAPI MakeMailslot(LPCWSTR);
BOOL WINAPI WriteServerSlot(LPCWSTR, double*);
BOOL WINAPI ReadClientSlot(HANDLE, double*);

// Создание сервера почтового ящика (Mailslot)
HANDLE WINAPI MakeMailslot(LPCWSTR SlotName) {
	// Создание дескриптора почтового ящика
	HANDLE hSlot = CreateMailslot(SlotName, 0, MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);
	// Проверка на ошибку при создании
	if (hSlot == INVALID_HANDLE_VALUE) {
		std::wstring data = L"CreateMailslot failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return NULL;
	}
	else return hSlot;
}

// Запись в серверный Mailslot
BOOL WINAPI WriteServerSlot(LPCWSTR SlotName, double* Message) {
	// Создание файла
	HANDLE hFile = CreateFile(SlotName, GENERIC_WRITE, FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	// Проверка на ошибку при создании
	if (hFile == INVALID_HANDLE_VALUE) {
		std::wstring data = L"CreateFile failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	// Запись в Mailslot
	DWORD cbWritten;
	BOOL fResult = WriteFile(hFile, Message, sizeof(double) * 4, &cbWritten, (LPOVERLAPPED)NULL);
	CloseHandle(hFile);
	// Проверка на ошибку при записи
	if (!fResult) {
		std::wstring data = L"WriteFile failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else return TRUE;
}

// Чтение из клиентского Mailslot
BOOL WINAPI ReadClientSlot(HANDLE hSlot, double* resBuff) {
	std::wstring data;
	DWORD cbMessage = 0, cMessage = 0, cbRead = 0;
	BOOL fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);
	if (!fResult) {
		data = L"GetMailslotInfo failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ClientReadSlot"));
	if (!hEvent) {
		data = L"CreateEvent failed with " + std::to_wstring(GetLastError());
		MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	if (cbMessage != MAILSLOT_NO_MESSAGE) {
		DWORD cAllMessages = cMessage;
		while (cMessage != 0) {
			OVERLAPPED ov;
			ov.Offset = 0;
			ov.OffsetHigh = 0;
			ov.hEvent = hEvent;
			fResult = ReadFile(hSlot, resBuff, cbMessage, &cbRead, &ov);
			if (!fResult) {
				data = L"ReadFile failed with " + std::to_wstring(GetLastError());
				MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
				delete resBuff;
				return FALSE;
			}

			fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);
			if (!fResult) {
				data = L"GetMailslotInfo failed with " + std::to_wstring(GetLastError());
				MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
				return FALSE;
			}
		}
	}
	CloseHandle(hEvent);
	return TRUE;
}

#endif