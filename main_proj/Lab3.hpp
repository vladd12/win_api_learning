/*----------Lab3.hpp----------*/
#ifndef LAB3
#define LAB3

#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>
#include <Process.h>

namespace lab3 {
	// Объявления функций
	void Lab3Exec(HWND hw);
	void UsingCSSync(HWND hw);
	void ThreadWorkCS(LPVOID arg);
	void UsingSemaphoreSync(HWND hw);
	void ThreadWorkSemaphore(LPVOID arg);
	void UsingMutexSync(HWND hw);
	void UsingEventSync(HWND hw);
	void ThreadDataShow(LPVOID arg);

	// Структура для передачи в метод потоков
	struct ThreadData {
		DWORD _num;
		UINT _outputNum;
		CRITICAL_SECTION* _csPtr;
		std::wofstream* _wfilePtr;
	};

	// Функция для выполнения задачи лабораторной работы
	void Lab3Exec(HWND hw) {
		UsingMutexSync(hw);
		UsingCSSync(hw);
		UsingSemaphoreSync(hw);
		UsingEventSync(hw);
	}

	// Синхронизация через критические секции
	void UsingCSSync(HWND hw) {
		// Инициализация критической секции, запуск потоков
		CRITICAL_SECTION csInstance;
		InitializeCriticalSection(&csInstance);

		// Открытие файла для очистки перед работой потоков
		std::wofstream fileWrite;
		std::locale defaultLocale("");
		fileWrite.imbue(defaultLocale);
		fileWrite.open("text.txt");
		fileWrite << L'\n';

		// Запускаем фиксированное кол-во потоков
		const UINT THREADCOUNT = 5;
		ThreadData ThreadArgs[THREADCOUNT];
		for (UINT i = 0; i < THREADCOUNT; i++) {
			// Создание потоков
			ThreadArgs[i]._num = i + 1;
			ThreadArgs[i]._outputNum = THREADCOUNT;
			ThreadArgs[i]._csPtr = &csInstance;
			ThreadArgs[i]._wfilePtr = &fileWrite;
			_beginthread(ThreadWorkCS, 1024, &ThreadArgs[i]);
		}
		Sleep(THREADCOUNT * 10);
		fileWrite.close();
		DeleteCriticalSection(&csInstance);

		// Открытие и чтение содержимого файла после работы потоков
		std::wstringstream data;
		std::wifstream fileRead;
		fileRead.imbue(defaultLocale);
		fileRead.open("text.txt");
		data << fileRead.rdbuf();
		fileRead.close();

		// Вывод результатов работы на экран
		std::wstring result = L"Содержимое файла text.txt:\n" + data.str();
		MessageBox(hw, result.c_str(), L"Синхронизация. Критические секции", MB_OK);
		
	}

	// Функция синхронизации потоков через критические секции
	void ThreadWorkCS(LPVOID arg) {
		ThreadData* data = static_cast<ThreadData*>(arg);
		EnterCriticalSection(data->_csPtr);
		for (UINT i = 0; i < data->_outputNum; i++) {
			*(data->_wfilePtr) << data->_num << L' ';
		}
		*(data->_wfilePtr) << L'\n';
		LeaveCriticalSection(data->_csPtr);
	}

	// Синхронизация через семафоры
	void UsingSemaphoreSync(HWND hw) {
		// Создание и проверка на создание семафора
		const UINT MAX_SEM_COUNT = 5, THREADCOUNT = 10;
		HANDLE ghSemaphore = CreateSemaphore(NULL, MAX_SEM_COUNT, MAX_SEM_COUNT, L"LabSemaphore");
		if (ghSemaphore == NULL) {
			std::wstring error = L"CreateSemaphore error: " + std::to_wstring(GetLastError());
			MessageBox(hw, error.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			exit(1);
		}

		// Создание потоков
		for (UINT i = 0; i < THREADCOUNT; i++) {
			_beginthread(ThreadWorkSemaphore, 1024, ghSemaphore);
		}
		
		// Продолжение работы
		MessageBox(hw, L"После закрытия этого окна дескриптор семафора будет закрыт.", L"Синхронизация. Семафоры", MB_OK);
		CloseHandle(ghSemaphore);
	}

	// Функция синхронизации потоков через семафоры
	void ThreadWorkSemaphore(LPVOID arg) {
		HANDLE ghSemaphore = arg;
		BOOL bContinue = TRUE;
		while (bContinue) {
			// Попытка использовать семафор
			DWORD dwWaitResult = WaitForSingleObject(ghSemaphore, 100L);
			std::wstring data = L"";

			switch (dwWaitResult) {
			// Был подан сигнал семафору
			case WAIT_OBJECT_0:
				data = L"Поток " + std::to_wstring(GetCurrentThreadId()) + L": поток получил ресурс семафоры.";
				MessageBox(NULL, data.c_str(), L"Синхронизация. Семафоры", MB_OK);
				bContinue = FALSE;
				Sleep(5);
				ReleaseSemaphore(ghSemaphore, 1, NULL);
				break;
			// Сигнал не дошёл до семафора
			case WAIT_TIMEOUT:
				data = L"Поток " + std::to_wstring(GetCurrentThreadId()) + L": время ожидания ресурса истекло.";
				MessageBox(NULL, data.c_str(), L"Ожидание", MB_OK);
				break;
			}
		}
	}

	// Синхронизация через мьютекс
	void UsingMutexSync(HWND hw) {
		// Создание и проверка на создание мьютекса
		HANDLE ghMutex = CreateMutex(NULL, FALSE, L"LabMutex");
		if (ghMutex == NULL) {
			std::wstring error = L"CreateMutex error: " + std::to_wstring(GetLastError());
			MessageBox(hw, error.c_str(), L"Ошибка!", MB_OK | MB_ICONERROR);
			exit(1);
		}

		// Если мьютекс уже был создан в другом экземпляре программы
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			MessageBox(hw, L"Приложение уже было запущено!\nДанное приложение будет закрыто.", L"Ошибка!", MB_OK | MB_ICONERROR);
			exit(1);
		}

		// После закрытия этого окна мьютекс будет освобождён
		MessageBox(hw, L"После закрытия этого окна дескриптор мьютекса будет закрыт.", L"Синхронизация. Мьютекс", MB_OK );
		CloseHandle(ghMutex);
	}

	// Синхронизация через событие
	void UsingEventSync(HWND hw) {
		// Создаём событие и потоки
		const UINT THREADCOUNT = 5;
		HANDLE ev = CreateEvent(NULL, TRUE, FALSE, L"LabEventSync");

		MessageBox(hw, L"После нажатия кнопки запустятся потоки", L"Синхронизация. События", MB_OK);
		for (UINT iter = 0; iter < THREADCOUNT; iter++) {
			_beginthread(ThreadDataShow, 1024, ev);
		}
	}

	// Вывод ID каждого потока через MsgBox после активации события
	void ThreadDataShow(LPVOID arg) {
		HANDLE event = arg;
		DWORD dwWaitResult = WaitForSingleObject(event, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			std::wstring threadData = L"ID текущего потока: " + std::to_wstring(GetCurrentThreadId());
			MessageBox(NULL, threadData.c_str(), L"Синхронизация. События", MB_OK);
		}
	}

}

#endif