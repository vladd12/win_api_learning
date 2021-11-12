#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// Объявления функций
HANDLE CreateFileMap(LPWSTR);
void ReadServerFileMap(vector<int>&);
void WriteClientFileMap(vector<int>&);

// Глобальные переменные
HANDLE evClientRead = NULL, evServerRead = NULL,
evClientSend = NULL, evMapFileInit = NULL;
LPWSTR ServerMapFileName = L"Local\\MyFileMappingObject";
LPVOID fileBuf = NULL;

// Основная функция программы
int wmain(int argc, WCHAR* argv[]) {
	// Создать события
	evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
	evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
	evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
	evMapFileInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");

	// Создать pipe и дополнительные переменные
	wcout << ServerMapFileName << L'\n';
	vector<int> data;
	HANDLE hMapFile = CreateFileMap(ServerMapFileName);
	DWORD counter = 0;
	SetEvent(evMapFileInit);

	while (counter <= 3) {
		// Начать чтение из канала
		SetEvent(evServerRead);
		// Ждём активации события отправки сообщения клиентом
		DWORD dwWaitResult = WaitForSingleObject(evClientSend, 1000);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Читаем данные из канала
			counter = 0;
			ReadServerFileMap(data);
			ResetEvent(evClientSend);
			ResetEvent(evServerRead);

			// Записываем массив в вектор и сортируем его
			sort(data.begin(), data.end());

			// Результат отправляем клиенту
			WriteClientFileMap(data);
			SetEvent(evClientRead);
		}
		else {
			wcout << L"Waiting data...\n";
			counter++;
		}
	}

	// Закрываем отображение файла в адресном пространстве процесса
	if (UnmapViewOfFile(fileBuf) == NULL) {
		wcout << L"UnmapViewOfFile failed with " << GetLastError() << endl;
	}

	// Очищаем память, закрываем дескрипторы
	CloseHandle(hMapFile);
	return 0;
}

// Функция для создания дескриптора канала
HANDLE CreateFileMap(LPWSTR FileName) {
	// Создаём файловое отображение
	const UINT BUFSIZE = 256;
	HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFSIZE, FileName);
	// Отображаем представление файла в адресное пространство процесса
	fileBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUFSIZE);
	return hMapFile;
}

// Сервер читает данные из файлового отображения
void ReadServerFileMap(vector<int>& in) {
	// Копируем первый элемент - размер блока
	int size;
	CopyMemory(&size, fileBuf, sizeof(int));
	wcout << L"Size of bucket: " << size << L";\nBucket: ";

	// Копируем элементы блока в динамический массив
	int *buf = new int[size];
	CopyMemory(buf, fileBuf, sizeof(int) * size);

	// Из динамического массива копируем элементы в вектор
	// и выводим полученные элементы в консоль
	for (int i = 0; i < size; i++) {
		wcout << buf[i] << L' ';
		in.push_back(buf[i]);
	}
	wcout << L"\nSorted bucket: ";
	delete[] buf;
}

// Сервер отправляет данные в файловое отображение
void WriteClientFileMap(vector<int>& in) {
	// Создаём новый динамический массив
	int size = in.size();
	int* buf = new int[size];

	// Копируем в него элементы из вектора по порядку 
	for (int i = 0; i < size; i++) {
		buf[i] = in[i];
		wcout << buf[i] << L' ';
	}
	wcout << L'\n';

	// Копируем результат в файл
	CopyMemory(fileBuf, buf, sizeof(int) * size);
	in.clear();
	delete[] buf;
}
