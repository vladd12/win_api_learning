#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <windows.h>

namespace Mailslot {
	using namespace std;

	void MailslotMain();
	void ReadFile(vector<int>&);
	void InitSort(const UINT, const vector<int>&);
	void ClientThread();
	HANDLE MakeMailslot(LPCWSTR);
	void SendToMailslot(vector<int>&, vector<int>&);
	void WriteServerSlot(LPCWSTR, vector<int>&);
	void ReadClientSlot(HANDLE, vector<int>&);

	// Глобальные переменные
	LPWSTR ClientSlotName = L"\\\\.\\mailslot\\Client";
	LPWSTR ServerSlotName = L"\\\\.\\mailslot\\Server";
	HANDLE evClientRead = NULL, evServerRead = NULL, evGlobalRead = NULL;
	vector<int> globalRes;

	// Главная функция, вызывается после нажатия кнопки
	void MailslotMain() {
		// Создать события
		evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
		evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
		evGlobalRead = CreateEvent(NULL, FALSE, FALSE, L"GlobalRead");

		// Чтение из файла
		vector<int> input;
		ReadFile(input);

		// Запуск сортировки
		const UINT BLOCK_NUMS = 3;
		InitSort(BLOCK_NUMS, input);
	}

	// Функция для чтения чисел из файла в вектор
	void ReadFile(vector<int>& IO) {
		wifstream fileRead;
		locale defaultLocale("");
		wstringstream data;
		fileRead.imbue(defaultLocale);
		fileRead.open("text.txt");
		data << fileRead.rdbuf();
		fileRead.clear();
		fileRead.seekg(0, ios::beg);
		int n;
		while (fileRead >> n) IO.push_back(n);
		fileRead.close();
		MessageBox(NULL, data.str().c_str(), L"Входной массив", MB_OK);
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

		// Запуск потока для инициализации клиентского почтового ящика
		thread thr1(ClientThread);

		// Находим максимальный размер блока
		max = -1;
		for (UINT i = 0; i < buckets.size(); i++) {
			if ((int)buckets[i].size() > max) max = buckets[i].size();
		}

		// Посылаем блоки на сортировку
		vector<int> res;
		for (UINT i = 0; i < buckets.size(); i++) {
			SendToMailslot(buckets[i], res);
		}

		// Вывод результата
		for (UINT i = 0; i < res.size(); i++) {
			data << res[i] << L' ';
		}
		MessageBox(NULL, data.str().c_str(), L"Результат", MB_OK);
		thr1.join();
	}

	// Поток, отвечающий за чтение сообщений из клиентского
	void ClientThread() {
		HANDLE hSlot = MakeMailslot(ClientSlotName);
		size_t counter = 0;

		// Ждём активации чтения клиентом 3 секунд, потом закрываемся
		while (counter <= 3) {
			DWORD dwWaitResult = WaitForSingleObject(evClientRead, 1000);
			// Клиент читает сообщение от сервера
			if (dwWaitResult == WAIT_OBJECT_0) {
				ReadClientSlot(hSlot, globalRes);
				SetEvent(evGlobalRead);
				counter = 0;
				ResetEvent(evClientRead);
			}
			// Если сообщений нет, увеличиваем значение
			else counter++;
		}

		// Закрываем дескриптор клиентского слота
		CloseHandle(hSlot);
	}
	
	// Функция для создания дескриптора слота
	HANDLE MakeMailslot(LPCWSTR sName) {
		HANDLE hSlot = CreateMailslot(sName, 0, MAILSLOT_WAIT_FOREVER, (LPSECURITY_ATTRIBUTES)NULL);
		return hSlot;
	}

	// Функция для запуска процесса и синхронизации данных
	void SendToMailslot(vector<int>& in, vector<int>& out) {
		// Параметры сервера
		const UINT BLOCK_SIZE = in.size();
		wstring params = wstring(ServerSlotName) + L' '+ to_wstring(BLOCK_SIZE);
		LPWSTR sParams = const_cast<wchar_t*>(params.c_str());

		// Создаём процесс
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL result = CreateProcess(L"LabOS4-1.exe", sParams, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		if (!result) {
			std::wstring data = L"CreateProcess failed with " + std::to_wstring(GetLastError());
			MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			return;
		}

		// Ждём активации события чтения сервером
		DWORD dwWaitResult = WaitForSingleObject(evServerRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Пишем серверу
			WriteServerSlot(ServerSlotName, in);
		}

		// Ждём окончания процесса
		dwWaitResult = WaitForSingleObject(pi.hProcess, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
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
	
	// Функция для отправки данных
	void WriteServerSlot(LPCWSTR sName, vector<int>& msg) {
		// Создание файла
		HANDLE hFile = CreateFile(sName, GENERIC_WRITE, FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

		// Копирование данных в динамический массив
		UINT bSize = msg.size();
		int* data = new int[bSize];
		for (UINT i = 0; i < bSize; i++) {
			data[i] = msg[i];
		}

		// Запись в Mailslot
		DWORD cbWritten;
		BOOL fResult = WriteFile(hFile, data, sizeof(int) * bSize, &cbWritten, (LPOVERLAPPED)NULL);
		CloseHandle(hFile);
		if (bSize == 1) delete data;
		else delete[] data;
	}

	// Функция для принятия данных
	void ReadClientSlot(HANDLE hSlot, vector<int>& res) {
		std::wstring data;
		DWORD cbMessage = 0, cMessage = 0, cbRead = 0;
		GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);
		HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, TEXT("ClientReadSlot"));

		if (cbMessage != MAILSLOT_NO_MESSAGE) {
			DWORD cAllMessages = cMessage;
			while (cMessage != 0) {
				OVERLAPPED ov;
				ov.Offset = 0;
				ov.OffsetHigh = 0;
				ov.hEvent = hEvent;

				UINT num = cbMessage / sizeof(int);
				int* data = new int[num];
				ReadFile(hSlot, data, cbMessage, &cbRead, &ov);
				GetMailslotInfo(hSlot, (LPDWORD)NULL, &cbMessage, &cMessage, (LPDWORD)NULL);

				for (UINT i = 0; i < num; i++) {
					res.push_back(data[i]);
				}
				delete[] data;
			}
		}
		CloseHandle(hEvent);
	}

}