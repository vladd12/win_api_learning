#include <iostream>
#include <string>
#include <windows.h>

// Объявления функций
double CalcDet(double*);
HANDLE CreatePipe(LPWSTR PipeName);
void ReadFromPipe(HANDLE hPipe, double* buf);
void WriteToPipe(HANDLE hPipe, double* message);

// Глобальные переменные
HANDLE evClientRead = NULL, evServerRead = NULL,
evClientSend = NULL, evPipeInit = NULL;
DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;

// Основная функция программы
int wmain(int argc, WCHAR* argv[]) {
	// Создать события
	evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
	evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
	evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
	evPipeInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");

	// Создать pipe и дополнительные переменные
	LPWSTR ServerPipeName = argv[0];
	std::wcout << ServerPipeName << L'\n';
	double* data = new double[4];
	HANDLE hPipe = CreatePipe(ServerPipeName);
	DWORD counter = 0;

	while (counter <= 5) {
		// Начать чтение из канала
		SetEvent(evServerRead);
		// Ждём активации события отправки сообщения клиентом
		DWORD dwWaitResult = WaitForSingleObject(evClientSend, 1000);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Читаем данные из канала
			counter = 0;
			ReadFromPipe(hPipe, data);
			ResetEvent(evClientSend);
			ResetEvent(evServerRead);

			// Вычисление определителя матрицы 2х2
			// и запись в канал
			double result = CalcDet(data);
			WriteToPipe(hPipe, &result);
			SetEvent(evClientRead);
		}
		else {
			std::wcout << L"Waiting data...\n";
			counter++;
		}
	}

	// Очищаем память, закрываем дескрипторы
	CloseHandle(hPipe);
	delete data;
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
	// Проверка на ошибки при создании
	if (hPipe == INVALID_HANDLE_VALUE) {
		std::wcout << L"CreateNamedPipe failed with " << GetLastError() << std::endl;
		return NULL;
	}

	// Ждём подключения клиента
	SetEvent(evPipeInit);
	BOOL fConnected = ConnectNamedPipe(hPipe, NULL);
	if (fConnected == TRUE) return hPipe; 
	else {
		std::wcout << L"ERROR_PIPE_CONNECTED\n";
		return NULL;
	}
}

// Сервер читает данные из канала
void ReadFromPipe(HANDLE hPipe, double* buf) {
	BOOL fSuccess = ReadFile(hPipe, buf, 4 * sizeof(double), &cbBytesRead, NULL);
	if (!fSuccess || cbBytesRead == 0) {
		if (GetLastError() == ERROR_BROKEN_PIPE) {
			std::wcout << L"ReadFromPipe: Client disconnected\n";
		} else {
			std::wcout << L"ReadFromPipe: ReadFile failed with " << GetLastError() << std::endl;
		}
	}
}

// Функция подсчёта определителя матрицы 2х2
double CalcDet(double* inVal) {
	double result = inVal[0] * inVal[3] - inVal[1] * inVal[2];
	std::wcout << result << std::endl;
	return result;
}

// Сервер отправляет данные в канал
void WriteToPipe(HANDLE hPipe, double* message) {
	cbReplyBytes = sizeof(double);
	BOOL fSuccess = WriteFile(hPipe, message, cbReplyBytes, &cbWritten, NULL);
	if (!fSuccess || cbReplyBytes != cbWritten) {
		std::wcout << L"InstanceThread WriteFile failed with " << GetLastError() << std::endl;
	}
}
