#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

HANDLE MakeMailslot(LPCWSTR);
void WriteClientSlot(LPCWSTR, int*, UINT&);
BOOL ReadServerSlot(HANDLE, int*);
void Sort(vector<int>&, vector<int>&);

// Глобальные переменные
LPWSTR ClientSlotName = L"\\\\.\\mailslot\\Client";
HANDLE evClientRead = NULL, evServerRead = NULL;

// Основная функция программы
int wmain(int argc, WCHAR* argv[]) {
	// Создать события
	evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
	evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");

	// Создать mailslot и активировать событие
	LPWSTR ServerSlotName = argv[0];
	LPWSTR strSize = argv[1];
	wcout << argv[0] << L' ' << argv[1] << L'\n';
	HANDLE hSlot = MakeMailslot(ServerSlotName);
	UINT arrSize = _wtoi(strSize);
	SetEvent(evServerRead);

	// Начать чтение mailslot
	int* data = new int[arrSize];
	while(!ReadServerSlot(hSlot, data));
	ResetEvent(evServerRead);

	// Записываем массив в вектор и сортируем его
	vector<int> in;
	for (UINT i = 0; i < arrSize; i++) {
		in.push_back(data[i]);
	}
	std::sort(in.begin(), in.end());
	for (UINT i = 0; i < arrSize; i++) {
		data[i] = in[i];
	}

	// Отправляем отсортированный массив клиенту
	WriteClientSlot(ClientSlotName, data, arrSize);
	SetEvent(evClientRead);

	// Очищаем память, закрываем дескрипторы
	CloseHandle(hSlot);
	delete[] data;
	return 0;
}

// Создание сервера почтового ящика (Mailslot)
HANDLE MakeMailslot(LPCWSTR sName) {
	// Создание дескриптора почтового ящика
	HANDLE hSlot = CreateMailslot(sName, 0, MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);
	return hSlot;
}

// Запись в клиентский Mailslot
void WriteClientSlot(LPCWSTR SlotName, int* Message, UINT& size) {
	// Создание файла
	HANDLE hFile = CreateFile(SlotName, GENERIC_WRITE, FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	// Запись в Mailslot
	DWORD cbWritten;
	WriteFile(hFile, Message, sizeof(int) * size, &cbWritten, (LPOVERLAPPED)NULL);
	CloseHandle(hFile);
}

// Чтение из серверного Mailslot
BOOL ReadServerSlot(HANDLE hSlot, int* inBuffer) {
	// Читаем информацию о сообщения
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ServerReadSlot"));
	DWORD cbMessage = 0, cMessage = 0, cbRead = 0;
	BOOL fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);

	// Если сообщений нет
	if (cbMessage == MAILSLOT_NO_MESSAGE) {
		wcout << L"Waiting for a message...\n";
		Sleep(500);
		return FALSE;
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
			fResult = GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);
		}
	}
	CloseHandle(hEvent);
	return TRUE;
}