/*----------Lab4Mailslot.hpp----------*/
#ifndef LAB4MAILSLOT
#define LAB4MAILSLOT

#include <string>
#include <sstream>
#include <thread>
#include <windows.h>
#include <process.h>
#include <strsafe.h>
#include <tchar.h>
#include "Matrix.hpp"
#include "MailslotFuncs.hpp"

namespace lab4_1 {
	// Объявления функций
	void MailslotMain(HWND);
	double MatrixDeterminant(Matrix&, UINT&);
	double CreateMatrixProcess(Matrix&, UINT&);
	void ClientThread();

	// Глобальные переменные
	LPWSTR ClientSlotName = L"\\\\.\\mailslot\\Client";
	LPWSTR ServerSlotName = L"\\\\.\\mailslot\\Server";
	HANDLE evClientRead = NULL, evServerRead = NULL, evGlobalRead = NULL;
	double globalRes = 0;

	// Главная функция
	void MailslotMain(HWND hw) {
		// Создать события
		evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
		evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");
		evGlobalRead = CreateEvent(NULL, FALSE, FALSE, L"GlobalRead");

		// Создать матрицу и необходимые переменные, 
		// начать работу дополнительного потока
		Matrix input(4, 4);
		UINT servNum = 0;
		MessageBox(NULL, input.ToWString(input).c_str(), L"Матрица", MB_OK);
		std::thread thr1(ClientThread);
		double result = MatrixDeterminant(input, servNum);
		MessageBox(NULL, std::to_wstring(result).c_str(), L"Результат", MB_OK);

		// Закрыть дескрипторы событий после завершения всех потоков
		thr1.join();
		CloseHandle(evClientRead);
		CloseHandle(evServerRead);
		CloseHandle(evGlobalRead);
	}

	// Поток, отвечающий за чтение сообщений из клиентского
	void ClientThread() {
		HANDLE hSlot = MakeMailslot(ClientSlotName);
		size_t counter = 0;
		double* msg = new double;

		// Ждём активации чтения клиентом 5 секунд, потом закрываемся
		while (counter <= 5) {
			DWORD dwWaitResult = WaitForSingleObject(evClientRead, 1000);
			// Клиент читает сообщение от сервера
			if (dwWaitResult == WAIT_OBJECT_0) {
				ReadClientSlot(hSlot, msg);
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
		CloseHandle(hSlot);
	}

	// Функция для поиска определителя матрицы
	double MatrixDeterminant(Matrix& mat, UINT& num) {
		if (mat.rows == 2 && mat.cols == 2) {
			num++;
			return CreateMatrixProcess(mat, num);
		} else {
			double res = 0, flag = 1;
			for (size_t i = 0; i < mat.cols; i++) {
				res = res + mat.GetCell(0, i) * MatrixDeterminant(mat.Exclude(mat, 0, i), num) * flag;
				flag = flag * (-1);
			}
			return res;
		}
	}

	// Создать процесс, которому будет передана матрица 2х2 для расчёта
	double CreateMatrixProcess(Matrix& mat, UINT& num) {
		// Имя сервера
		std::wstring servName = std::wstring(ServerSlotName) + std::to_wstring(num);
		LPWSTR sName = const_cast<wchar_t*>(servName.c_str());

		// Формируем линейный массив из элементов матрицы
		double arr[4];
		arr[0] = mat.GetCell(0, 0);
		arr[1] = mat.GetCell(0, 1);
		arr[2] = mat.GetCell(1, 0);
		arr[3] = mat.GetCell(1, 1);

		// Создаём процесс
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(si);
		BOOL result = CreateProcess(L"Lab-4-1.exe", sName, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
		if (!result) {
			std::wstring data = L"CreateProcess failed with " + std::to_wstring(GetLastError());
			MessageBox(NULL, data.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			return FALSE;
		}

		// Ждём активации чтения сервером
		DWORD dwWaitResult = WaitForSingleObject(evServerRead, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Пишем серверу
			WriteServerSlot(sName, arr);
			WriteServerSlot(sName, arr);
		}

		// Ждём окончания процесса
		dwWaitResult = WaitForSingleObject(pi.hProcess, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
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

}

#endif
