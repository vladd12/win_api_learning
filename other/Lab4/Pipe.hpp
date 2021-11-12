#pragma once

#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>
#include "Mailslot.hpp"

namespace Pipe {
	using namespace std;

	void PipeMain();
	void InitSort(const UINT, const vector<int>&);
	void ServerInitPipeThread();
	void ClientThread();
	void SendToPipe(vector<int>&, vector<int>&);
	void WriteServerPipe(HANDLE, vector<int>&);
	void ReadClientPipe(HANDLE, vector<int>&);

	// Глобальные переменные
	HANDLE evClientRead = NULL, evServerRead = NULL,
	evClientSend = NULL, evPipeInit = NULL, evGlobalRead = NULL;
	HANDLE openPipe = NULL;
	vector<int> globalRes;
	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;

	// Главная функция
	void PipeMain() {
		// Создать события
		evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
		evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
		evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
		evPipeInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");
		evGlobalRead = CreateEvent(NULL, FALSE, FALSE, L"GlobalRead");

		// Чтение из файла
		vector<int> input;
		Mailslot::ReadFile(input);

		// Запуск сортировки
		const UINT BLOCK_NUMS = 3;
		InitSort(BLOCK_NUMS, input);

		// Закрыть дескрипторы событий после завершения всех потоков
		CloseHandle(openPipe);
		CloseHandle(evClientRead);
		CloseHandle(evClientSend);
		CloseHandle(evServerRead);
		CloseHandle(evPipeInit);
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
		thread thr1(ServerInitPipeThread);
		thread thr2(ClientThread);

		// Находим максимальный размер блока
		max = -1;
		for (UINT i = 0; i < buckets.size(); i++) {
			if ((int)buckets[i].size() > max) max = buckets[i].size();
		}

		// Посылаем блоки на сортировку
		vector<int> res;
		for (UINT i = 0; i < buckets.size(); i++) {
			SendToPipe(buckets[i], res);
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
	void ServerInitPipeThread() {
		LPWSTR PipeName = L"\\\\.\\pipe\\SharedPipe";
		// Создаём процесс для сервера
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		CreateProcess(L"LabOS4-2.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		// Ждём активации создания сервера
		DWORD dwWaitResult = WaitForSingleObject(evPipeInit, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Открываем глобальный дескриптор файла
			openPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		}

		// Ждём окончания процесса
		dwWaitResult = WaitForSingleObject(pi.hProcess, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
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
				ReadClientPipe(openPipe, globalRes);
				SetEvent(evGlobalRead);
				counter = 0;
				ResetEvent(evClientRead);
			}
			// Если сообщений нет, увеличиваем значение
			else counter++;
		}
	}

	// Функция для запуска процесса и синхронизации данных
	void SendToPipe(vector<int>& in, vector<int>& out) {
		// Ждём активации события чтения сервером
		DWORD dwWaitResult = WaitForSingleObject(evServerRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Пишем серверу
			WriteServerPipe(openPipe, in);
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
	void WriteServerPipe(HANDLE hPipe, vector<int>& msg) {
		// Первый элемент массива - его длина
		UINT bSize = msg.size();
		cbReplyBytes = sizeof(int) * (bSize + 1);
		int* data = new int[bSize + 1];
		data[0] = bSize;
		for (UINT i = 0; i < bSize; i++) {
			data[i + 1] = msg[i];
		}

		WriteFile(hPipe, data, cbReplyBytes, &cbWritten, NULL);
		SetEvent(evClientSend);
		delete[] data;
	}

	// Считать отсортированный блок из канала
	void ReadClientPipe(HANDLE hPipe, vector<int>& res) {
		UINT bSize = (cbReplyBytes - sizeof(int)) / sizeof(int);
		int* data = new int[bSize];
		BOOL fSuccess = ReadFile(hPipe, data, cbReplyBytes - sizeof(int), &cbBytesRead, NULL);
		if (fSuccess && cbBytesRead != 0) {
			for (UINT i = 0; i < bSize; i++) {
				res.push_back(data[i]);
			}
		}
		delete[] data;
	}

}