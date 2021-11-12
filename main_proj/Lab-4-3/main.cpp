#include <iostream>
#include <string>
#include <windows.h>

// Объявления функций
double CalcDet(double*);
HANDLE CreateMapFile(LPWSTR ServerMapFileName);
void ReadFromClient(HANDLE hMapFile, double* buf);
void WriteToClient(HANDLE hMapFile, double* message);

// Глобальные переменные
const UINT BUFSIZE = 256;
LPVOID fileBuf = NULL;
HANDLE evClientRead = NULL, evServerRead = NULL,
	evClientSend = NULL, evMapFileInit = NULL;

// Основная функция программы
int wmain(int argc, WCHAR* argv[]) {
	// Создать события
	evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
	evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
	evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
	evMapFileInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");

	// Создать fileMap и дополнительные переменные
	LPWSTR ServerMapFileName = argv[0];
	std::wcout << ServerMapFileName << L'\n';
	double* data = new double[4];
	HANDLE hMapFile = CreateMapFile(ServerMapFileName);
	SetEvent(evMapFileInit);
	DWORD counter = 0;

	while (counter <= 5) {
		// Начать чтение из файла
		SetEvent(evServerRead);
		// Ждём активации события отправки сообщения клиентом
		DWORD dwWaitResult = WaitForSingleObject(evClientSend, 1000);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Читаем данные из файла
			counter = 0;
			ReadFromClient(hMapFile, data);
			ResetEvent(evClientSend);
			ResetEvent(evServerRead);

			// Вычисление определителя матрицы 2х2
			double result = CalcDet(data);
			// Запись результата в канал
			WriteToClient(hMapFile, &result);
			SetEvent(evClientRead);
		}
		else {
			std::wcout << L"Waiting data...\n";
			counter++;
		}
	}


	// Закрываем отображение файла в адресном пространстве процесса
	if (UnmapViewOfFile(fileBuf) == NULL) {
		std::wcout << L"UnmapViewOfFile failed with " << GetLastError() << std::endl;
	}
	// Очищаем память, закрываем дескрипторы
	CloseHandle(hMapFile);
	delete data;
	return 0;
}

// Функция для создания дескриптора файлового отображения
HANDLE CreateMapFile(LPWSTR ServerMapFileName) {
	// Создаём файловое отображение
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFSIZE, ServerMapFileName
	);
	// Проверка на ошибки при создании
	if (hMapFile == NULL) {
		std::wcout << L"CreateFileMapping failed with " << GetLastError() << std::endl;
		return NULL;
	}
	// Отображаем представление файла в адресное пространство процесса
	fileBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZE);
	if (fileBuf == NULL) {
		std::wcout << L"MapViewOfFile failed with " << GetLastError() << std::endl;
		return NULL;
	}
	return hMapFile;
}

// Сервер читает данные из канала
void ReadFromClient(HANDLE hMapFile, double* buf) {
	// Копируем из файла пересланные данные
	CopyMemory((LPVOID)buf, fileBuf, sizeof(double) * 4);
}

// Функция подсчёта определителя матрицы 2х2
double CalcDet(double* inVal) {
	double result = inVal[0] * inVal[3] - inVal[1] * inVal[2];
	std::wcout << result << std::endl;
	return result;
}

// Сервер отправляет данные в канал
void WriteToClient(HANDLE hMapFile, double* message) {
	// Копируем результат в файл
	CopyMemory(fileBuf, (LPVOID)message, sizeof(double));
}
