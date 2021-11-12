/*----------Lab4FileMapping.hpp----------*/
#ifndef LAB4FILEMAP
#define LAB4FILEMAP

#include <string>
#include <sstream>
#include <thread>
#include <windows.h>
#include "Matrix.hpp"

namespace lab4_3 {
	// Объявления функций
	void FileMappingMain(HWND);
	double MatrixDetFileMap(Matrix&, UINT&);
	double MatrixToFileMap(Matrix&, UINT&);
	void ServerInitMapFileThread(LPWSTR lpszPipename);
	void ClientMapFileThread();
	void WriteToServer(HANDLE hPipe, double* message);
	void ReadFromServer(HANDLE hPipe, double* buf);

	// Глобальные переменные
	HANDLE evClientRead = NULL, evServerRead = NULL, evFileOpened = NULL,
		evClientSend = NULL, evMapFileInit = NULL, evGlobalRead = NULL;
	HANDLE openMapFile = NULL; LPVOID mapFileBuf = NULL;
	double globalRes = 0;

	// Главная функция
	void FileMappingMain(HWND hw) {
		// Создать события
		LPWSTR MapFileName = L"Local\\MyFileMappingObject";
		evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
		evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
		evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
		evMapFileInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");
		evFileOpened = CreateEvent(NULL, FALSE, FALSE, L"FileOpened");
		evGlobalRead = CreateEvent(NULL, FALSE, FALSE, L"GlobalRead");

		// Создать матрицу и необходимые переменные
		Matrix input(4, 4);
		UINT servNum = 0;
		MessageBox(NULL, input.ToWString(input).c_str(), L"Матрица", MB_OK);
		std::thread thr1(ServerInitMapFileThread, MapFileName);
		std::thread thr2(ClientMapFileThread);
		double result = MatrixDetFileMap(input, servNum);
		MessageBox(NULL, std::to_wstring(result).c_str(), L"Результат", MB_OK);

		// Закрыть дескрипторы событий после завершения всех потоков
		thr1.join();
		thr2.join();
		CloseHandle(openMapFile);
		CloseHandle(evClientRead);
		CloseHandle(evClientSend);
		CloseHandle(evServerRead);
		CloseHandle(evMapFileInit);
		CloseHandle(evGlobalRead);
	}

	// Функция для создания процесса сервера в отдельном потоке
	void ServerInitMapFileThread(LPWSTR MapFileName) {
		// Создаём процесс для сервера
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		std::wstring data;
		BOOL result = CreateProcess(L"Lab-4-3.exe", MapFileName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		if (!result) {
			data = L"CreateProcess failed with " + std::to_wstring(GetLastError());
			MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			return;
		}

		// Ждём активации создания сервера
		DWORD dwWaitResult = WaitForSingleObject(evMapFileInit, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Открываем глобальный дескриптор файла
			openMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MapFileName);
			// Проверка на ошибки при открытии файла
			if (openMapFile == NULL) {
				std::wstring data = L"OpenFileMapping failed with " + std::to_wstring(GetLastError());
				MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
				return;
			}

			// Отображаем представление файла в адресное пространство процесса
			mapFileBuf = MapViewOfFile(openMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
			if (mapFileBuf == NULL) {
				std::wstring data = L"MapViewOfFile failed with " + std::to_wstring(GetLastError());
				MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
				return;
			}
		}
		SetEvent(evFileOpened);

		// Ждём окончания процесса
		dwWaitResult = WaitForSingleObject(pi.hProcess, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Закрываем отображение файла в адресном пространстве процесса
			if (UnmapViewOfFile(mapFileBuf) == NULL) {
				std::wstring data = L"UnmapViewOfFile failed with " + std::to_wstring(GetLastError());
				MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			}
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	// Поток, отвечающий за чтение сообщений из клиентского
	void ClientMapFileThread() {
		size_t counter = 0;
		double* msg = new double;

		// Ждём активации чтения клиентом 5 секунд, потом закрываемся
		while (counter <= 5) {
			DWORD dwWaitResult = WaitForSingleObject(evClientRead, 1000);
			// Клиент читает сообщение от сервера
			if (dwWaitResult == WAIT_OBJECT_0) {
				ReadFromServer(openMapFile, msg);
				globalRes = *msg;
				SetEvent(evGlobalRead);
				counter = 0;
				ResetEvent(evClientRead);
			}
			// Если сообщений нет, увеличиваем значение
			else counter++;
		}

		// Очищаем память, закрываем дескрипторы
		delete msg;
	}

	// Функция для поиска определителя матрицы
	double MatrixDetFileMap(Matrix& mat, UINT& num) {
		if (mat.rows == 2 && mat.cols == 2) {
			num++;
			return MatrixToFileMap(mat, num);
		}
		else {
			double res = 0, flag = 1;
			for (size_t i = 0; i < mat.cols; i++) {
				res = res + mat.GetCell(0, i) * MatrixDetFileMap(mat.Exclude(mat, 0, i), num) * flag;
				flag = flag * (-1);
			}
			return res;
		}
	}

	// Создать процесс, которому будет передана матрица 2х2 для расчёта
	double MatrixToFileMap(Matrix& mat, UINT& num) {
		// Формируем линейный массив из элементов матрицы
		double arr[4];
		arr[0] = mat.GetCell(0, 0);
		arr[1] = mat.GetCell(0, 1);
		arr[2] = mat.GetCell(1, 0);
		arr[3] = mat.GetCell(1, 1);

		// Ждём открытия файлового отображения в данном процессе
		DWORD dwWaitResult = WaitForSingleObject(evFileOpened, INFINITE);

		// Ждём активации события чтения сервером
		dwWaitResult = WaitForSingleObject(evServerRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Пересылаем матрицу в сервер
			WriteToServer(openMapFile, arr);
			SetEvent(evClientSend);
		}

		// Ждём активации события чтения из глобальной переменной
		double retVal = 0;
		dwWaitResult = WaitForSingleObject(evGlobalRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			retVal = globalRes;
			globalRes = 0;
			ResetEvent(evGlobalRead);
		}

		// Вернуть значение
		SetEvent(evFileOpened);
		return retVal;
	}

	// Записать в канал матрицу
	void WriteToServer(HANDLE hPipe, double* message) {
		CopyMemory(mapFileBuf, (LPVOID)message, sizeof(double) * 4);
	}

	// Считать результат из канала
	void ReadFromServer(HANDLE hPipe, double* buf) {
		CopyMemory((LPVOID)buf, mapFileBuf, sizeof(double));
	}

}

#endif
