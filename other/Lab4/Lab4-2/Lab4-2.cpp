#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// Объявления функций
HANDLE CreatePipe(LPWSTR PipeName);
void ReadServerPipe(HANDLE, vector<int>&);
void WriteToPipe(HANDLE, vector<int>&);

// Глобальные переменные
HANDLE evClientRead = NULL, evServerRead = NULL,
evClientSend = NULL, evPipeInit = NULL;
DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;
LPWSTR ServerPipeName = L"\\\\.\\pipe\\SharedPipe";

// Основная функция программы
int wmain(int argc, WCHAR* argv[]) {
	// Создать события
	evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
	evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
	evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
	evPipeInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");

	// Создать pipe и дополнительные переменные
	wcout << ServerPipeName << L'\n';
	vector<int> data;
	HANDLE hPipe = CreatePipe(ServerPipeName);
	DWORD counter = 0;

	while (counter <= 3) {
		// Начать чтение из канала
		SetEvent(evServerRead);
		// Ждём активации события отправки сообщения клиентом
		DWORD dwWaitResult = WaitForSingleObject(evClientSend, 1000);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Читаем данные из канала
			counter = 0;
			ReadServerPipe(hPipe, data);
			ResetEvent(evClientSend);
			ResetEvent(evServerRead);

			// Записываем массив в вектор и сортируем его
			sort(data.begin(), data.end());
			WriteToPipe(hPipe, data);
			SetEvent(evClientRead);
		}
		else {
			wcout << L"Waiting data...\n";
			counter++;
		}
	}

	// Очищаем память, закрываем дескрипторы
	CloseHandle(hPipe);
	return 0;
}

// Функция для создания дескриптора канала
HANDLE CreatePipe(LPWSTR PipeName) {
	// Создаём канал
	const UINT BUFSIZE = 512;
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	hPipe = CreateNamedPipe(PipeName, PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, BUFSIZE, BUFSIZE, 0, NULL);

	// Ждём подключения клиента
	SetEvent(evPipeInit);
	BOOL fConnected = ConnectNamedPipe(hPipe, NULL);
	if (fConnected == TRUE) return hPipe;
	else {
		wcout << L"ERROR_PIPE_CONNECTED\n";
		return NULL;
	}
}

// Сервер читает данные из канала
void ReadServerPipe(HANDLE hPipe, vector<int>& in) {
	int size;
	BOOL fSuccess = ReadFile(hPipe, &size, sizeof(int), &cbBytesRead, NULL);
	wcout << L"Size of bucket: " << size << L";\nBucket: ";
	int* buf = new int[size];
	fSuccess = ReadFile(hPipe, buf, size * sizeof(int), &cbBytesRead, NULL);
	if (fSuccess && cbBytesRead != 0) {
		for (int i = 0; i < size; i++) {
			wcout << buf[i] << L' ';
			in.push_back(buf[i]);
		}
		wcout << L"\nSorted bucket: ";
	}
	delete[] buf;
}

// Сервер отправляет данные в канал
void WriteToPipe(HANDLE hPipe, vector<int>& in) {
	int size = in.size();
	cbReplyBytes = sizeof(int) * size;
	int* buf = new int[size];
	
	// Копируем в буфер 
	for (int i = 0; i < size; i++) {
		buf[i] = in[i];
		wcout << buf[i] << L' ';
	}
	wcout << L'\n';

	WriteFile(hPipe, buf, cbReplyBytes, &cbWritten, NULL);
	in.clear();
	delete[] buf;
}
