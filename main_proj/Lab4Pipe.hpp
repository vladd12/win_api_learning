/*----------Lab4Pipe.hpp----------*/
#ifndef LAB4PIPE
#define LAB4PIPE

#include <string>
#include <sstream>
#include <thread>
#include <windows.h>
#include "Matrix.hpp"

namespace lab4_2 {
	// Объявления функций
	void PipeMain(HWND);
	double MatrixDetPipe(Matrix&, UINT&);
	double SendMatrixToPipe(Matrix&, UINT&);
	void ServerInitPipeThread(LPWSTR lpszPipename);
	void ClientPipeThread();
	void ReadFromPipe(HANDLE hPipe, double* buf);
	void WriteToPipe(HANDLE hPipe, double* message);

	// Глобальные переменные
	HANDLE evClientRead = NULL, evServerRead = NULL,
		evClientSend = NULL, evPipeInit = NULL, evGlobalRead = NULL;
	HANDLE openPipe = NULL;
	double globalRes = 0;
	DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0;

	// Главная функция
	void PipeMain(HWND hw) {
		// Создать события
		LPWSTR PipeName = L"\\\\.\\pipe\\SharedPipe";
		evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
		evClientSend = CreateEvent(NULL, FALSE, FALSE, L"ClientWrite");
		evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
		evPipeInit = CreateEvent(NULL, FALSE, FALSE, L"ServerInit");
		evGlobalRead = CreateEvent(NULL, FALSE, FALSE, L"GlobalRead");

		// Создать матрицу и необходимые переменные
		Matrix input(4, 4);
		UINT servNum = 0;
		MessageBox(NULL, input.ToWString(input).c_str(), L"Матрица", MB_OK);
		std::thread thr1(ServerInitPipeThread, PipeName);
		std::thread thr2(ClientPipeThread);
		double result = MatrixDetPipe(input, servNum);
		MessageBox(NULL, std::to_wstring(result).c_str(), L"Результат", MB_OK);

		// Закрыть дескрипторы событий после завершения всех потоков
		thr1.join();
		thr2.join();
		CloseHandle(openPipe);
		CloseHandle(evClientRead);
		CloseHandle(evClientSend);
		CloseHandle(evServerRead);
		CloseHandle(evPipeInit);
		CloseHandle(evGlobalRead);
	}

	// Функция для создания процесса сервера в отдельном потоке
	void ServerInitPipeThread(LPWSTR PipeName) {
		// Создаём процесс для сервера
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		std::wstring data;
		BOOL result = CreateProcess(L"Lab-4-2.exe", PipeName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		if (!result) {
			data = L"CreateProcess failed with " + std::to_wstring(GetLastError());
			MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			return;
		}

		// Ждём активации создания сервера
		DWORD dwWaitResult = WaitForSingleObject(evPipeInit, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Открываем глобальный дескриптор файла
			openPipe = CreateFile(PipeName, GENERIC_READ | GENERIC_WRITE, 0,
				NULL, OPEN_EXISTING, 0, NULL);
		}

		// Ждём окончания процесса
		dwWaitResult = WaitForSingleObject(pi.hProcess, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
	}

	// Поток, отвечающий за чтение сообщений из клиентского
	void ClientPipeThread() {
		size_t counter = 0;
		double* msg = new double;

		// Ждём активации чтения клиентом 5 секунд, потом закрываемся
		while (counter <= 5) {
			DWORD dwWaitResult = WaitForSingleObject(evClientRead, 1000);
			// Клиент читает сообщение от сервера
			if (dwWaitResult == WAIT_OBJECT_0) {
				ReadFromPipe(openPipe, msg);
				globalRes = *msg;
				SetEvent(evGlobalRead);
				counter = 0;
				ResetEvent(evClientRead);
			}
			// Если сообщений нет, увеличиваем значение
			else {
				counter++;
			}
		}

		// Очищаем память, закрываем дескрипторы
		delete msg;
	}

	// Функция для поиска определителя матрицы
	double MatrixDetPipe(Matrix& mat, UINT& num) {
		if (mat.rows == 2 && mat.cols == 2) {
			num++;
			return SendMatrixToPipe(mat, num);
		}
		else {
			double res = 0, flag = 1;
			for (size_t i = 0; i < mat.cols; i++) {
				res = res + mat.GetCell(0, i) * MatrixDetPipe(mat.Exclude(mat, 0, i), num) * flag;
				flag = flag * (-1);
			}
			return res;
		}
	}

	// Создать процесс, которому будет передана матрица 2х2 для расчёта
	double SendMatrixToPipe(Matrix& mat, UINT& num) {
		// Формируем линейный массив из элементов матрицы
		double arr[4];
		arr[0] = mat.GetCell(0, 0);
		arr[1] = mat.GetCell(0, 1);
		arr[2] = mat.GetCell(1, 0);
		arr[3] = mat.GetCell(1, 1);

		// Ждём активации события чтения сервером
		DWORD dwWaitResult = WaitForSingleObject(evServerRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Пересылаем матрицу в сервер
			WriteToPipe(openPipe, arr);
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
		return retVal;
	}

	// Записать в канал матрицу
	void WriteToPipe(HANDLE hPipe, double* message) {
		cbReplyBytes = sizeof(double) * 4;
		BOOL fSuccess = WriteFile(hPipe, message, cbReplyBytes, &cbWritten, NULL);
		if (!fSuccess || cbReplyBytes != cbWritten) {
			std::wstring data = L"InstanceThread WriteFile failed with " + std::to_wstring(GetLastError());
			MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
		}
	}

	// Считать результат из канала
	void ReadFromPipe(HANDLE hPipe, double* buf) {
		BOOL fSuccess = ReadFile(hPipe, buf, sizeof(double), &cbBytesRead, NULL);
		if (!fSuccess || cbBytesRead == 0) {
			if (GetLastError() == ERROR_BROKEN_PIPE) {
				std::wstring data = L"ReadFromPipe: Client disconnected!\n";
				MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			}
			else {
				std::wstring data = L"ReadFromPipe: ReadFile failed with " + std::to_wstring(GetLastError());
				MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			}
		}
	}

}

#endif
