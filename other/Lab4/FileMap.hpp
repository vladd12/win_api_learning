#pragma once

#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
#include "Mailslot.hpp"

namespace FileMap {
	using namespace std;

	void FileMapMain();
	void InitSort(const UINT, const vector<int>&);
	void ServerInitFileMapThread();
	void ClientThread();
	void SendToFile(vector<int>&, vector<int>&);
	void WriteServerFileMap(vector<int>&);
	void ReadClientFileMap(vector<int>&);

	// Глобальные переменные
	HANDLE evClientRead = NULL, evServerRead = NULL, evFileOpened = NULL,
	evClientSend = NULL, evMapFileInit = NULL, evGlobalRead = NULL;
	LPVOID mapFileBuf = NULL;
	vector<int> globalRes; UINT bSize;

	// Главная функция
	void FileMapMain() {
		// Создать события
		evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
		evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
		evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
		evFileOpened = CreateEvent(NULL, FALSE, FALSE, L"FileOpened");
		evMapFileInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");
		evGlobalRead = CreateEvent(NULL, FALSE, FALSE, L"GlobalRead");

		// Чтение из файла
		vector<int> input;
		Mailslot::ReadFile(input);

		// Запуск сортировки
		const UINT BLOCK_NUMS = 3;
		InitSort(BLOCK_NUMS, input);

		// Закрыть дескрипторы после завершения всех потоков
		CloseHandle(evClientRead);
		CloseHandle(evClientSend);
		CloseHandle(evServerRead);
		CloseHandle(evMapFileInit);
		CloseHandle(evFileOpened);
		CloseHandle(evGlobalRead);
	}

	// Разбиение массива на блоки, вызов процессов для сортировки блоков
	void InitSort(const UINT bNums, const vector<int>& in) {
		// Инициализируем блоки для сортировки
		vector<vector<int>> buckets;
		for (UINT i = 0; i < bNums; i++) {
			buckets.push_back(vector<int>());
		}

		// Ищем min и max входного массива
		int min = in[0], max = in[0];
		for (UINT i = 1; i < in.size(); i++) {
			if (in[i] > max) max = in[i];
			else if (in[i] < min) min = in[i];
		}

		// Создаём массив границ
		wstringstream data;
		int* borders;
		UINT bordNums = bNums + 1;
		if (bordNums > 0) {
			int step = (max - min) / bNums;
			borders = new int[bordNums];
			borders[0] = min;
			borders[bordNums - 1] = max + 1;
			for (UINT i = 1; i < bordNums - 1; i++) {
				borders[i] = borders[i - 1] + step;
			}
		}

		// Растасовываем элементы исходного массива по блокам
		for (UINT i = 0; i < in.size(); i++) {
			for (UINT j = 1; j < bordNums; j++) {
				if (in[i] >= borders[j - 1] && in[i] < borders[j]) {
					buckets[j - 1].push_back(in[i]);
				}
			}
		}

		// Вывод блоков
		for (UINT i = 0; i < buckets.size(); i++) {
			for (UINT j = 0; j < buckets[i].size(); j++) {
				data << buckets[i][j] << L' ';
			}
			data << L'\n';
		}
		MessageBox(NULL, data.str().c_str(), L"Блоки", MB_OK);
		data.str(L"");

		// Запуск потока для инициализации клиента
		thread thr1(ServerInitFileMapThread);
		thread thr2(ClientThread);

		// Находим максимальный размер блока
		max = -1;
		for (UINT i = 0; i < buckets.size(); i++) {
			if ((int)buckets[i].size() > max) max = buckets[i].size();
		}

		// Посылаем блоки на сортировку
		vector<int> res;
		for (UINT i = 0; i < buckets.size(); i++) {
			SendToFile(buckets[i], res);
		}

		// Вывод результата
		for (UINT i = 0; i < res.size(); i++) {
			data << res[i] << L' ';
		}
		MessageBox(NULL, data.str().c_str(), L"Результат", MB_OK);
		thr1.join();
		thr2.join();
	}

	// Функция для создания процесса сервера в отдельном потоке
	void ServerInitFileMapThread() {
		// Создаём процесс для сервера
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		CreateProcess(L"LabOS4-3.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		// Ждём активации создания сервера
		LPWSTR MapFileName = L"Local\\MyFileMappingObject";
		HANDLE openMapFile = NULL;
		DWORD dwWaitResult = WaitForSingleObject(evMapFileInit, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Открываем глобальный дескриптор файла
			openMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapFileName);
			// Отображаем представление файла в адресное пространство процесса
			mapFileBuf = MapViewOfFile(openMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
		}
		SetEvent(evFileOpened);

		// Ждём окончания процесса
		dwWaitResult = WaitForSingleObject(pi.hProcess, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			UnmapViewOfFile(mapFileBuf);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			CloseHandle(openMapFile);
		}
	}

	// Поток, отвечающий за чтение сообщений из клиентского
	void ClientThread() {
		// Ждём активации чтения клиентом 3 секунд, потом закрываемся
		size_t counter = 0;
		while (counter <= 3) {
			DWORD dwWaitResult = WaitForSingleObject(evClientRead, 1000);
			// Клиент читает сообщение от сервера
			if (dwWaitResult == WAIT_OBJECT_0) {
				ReadClientFileMap(globalRes);
				SetEvent(evGlobalRead);
				counter = 0;
				ResetEvent(evClientRead);
			}
			// Если сообщений нет, увеличиваем значение
			else counter++;
		}
	}

	// Функция для запуска процесса и синхронизации данных
	void SendToFile(vector<int>& in, vector<int>& out) {
		// Ждём активации события чтения сервером
		DWORD dwWaitResult = WaitForSingleObject(evServerRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Пишем серверу
			WaitForSingleObject(evFileOpened, INFINITE);
			WriteServerFileMap(in);
			SetEvent(evClientSend);
			SetEvent(evFileOpened);
		}

		// Ждём активации события чтения из глобальной переменной
		dwWaitResult = WaitForSingleObject(evGlobalRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			for (UINT i = 0; i < globalRes.size(); i++) {
				out.push_back(globalRes[i]);
			}
			globalRes.clear();
			ResetEvent(evGlobalRead);
		}
	}

	// Записать в канал блок
	void WriteServerFileMap(vector<int>& msg) {
		// Создание динамического массива, из которого будут пересылаться данные
		bSize = msg.size();
		int* data = new int[bSize + 1];
		data[0] = bSize;
		for (UINT i = 0; i < bSize; i++) {
			data[i + 1] = msg[i];
		}

		// Копирование в файл и освобождение памяти
		CopyMemory(mapFileBuf, data, sizeof(int) * (bSize + 1));
		delete[] data;
	}

	// Считать отсортированный блок из канала
	void ReadClientFileMap(vector<int>& res) {
		// Копирование результата из файла в динамический массив
		int* data = new int[bSize];
		CopyMemory(data, mapFileBuf, sizeof(int) * bSize);

		// Копирование результата из динамического массива в вектор
		for (UINT i = 0; i < bSize; i++) {
			res.push_back(data[i]);
		}
		delete[] data;
	}

}