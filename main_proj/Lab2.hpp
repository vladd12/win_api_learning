/*----------Lab2.hpp----------*/
#ifndef LAB2
#define LAB2

#include <string>
#include <sstream>
#include <Windows.h>
#include <Shlwapi.h>
#include <Tlhelp32.h>

namespace lab2 {

	// Объявления функций
	void Lab2Exec(HWND hw);
	void ShowModulesInfo(HWND hw);
	std::wstring GetModuleInfo();
	std::wstring GetModuleInfo(LPCWSTR moduleName);
	std::wstring GetModuleInfo(HMODULE moduleHandle);
	std::wstring ModuleInfoToStr(LPCWSTR moduleFilePath, HMODULE moduleHandle);
	std::wstring handleToStr(HANDLE _handle);
	std::wstring CurProcInfo();
	void ShowListEnumAllProc(HWND hw, UINT procMax, BYTE modMax, BYTE threadMax);
	void ShowProcListModules(DWORD ProcID, std::wstringstream& wStream, const BYTE modMax);
	void ShowProcListThreads(DWORD ProcID, std::wstringstream& wStream, const BYTE threadMax);

	// Функция для выполнения задачи лабораторной работы
	void Lab2Exec(HWND hw) {
		ShowModulesInfo(hw);
		ShowListEnumAllProc(hw, 1000, 7, 4);
	}

	// Показать информацию о нужных модулях
	void ShowModulesInfo(HWND hw) {
		LPWSTR module2Name = L"kernel32.dll";
		HMODULE module3Handle = GetModuleHandle(L"user32.dll");
		std::wstring data = L"Информация о текущей программе:\n" + GetModuleInfo()
			+ L"\n\nИнформация о модуле по имени модуля:\n" + GetModuleInfo(module2Name)
			+ L"\n\nИнформация о модуле по дескриптору:\n" + GetModuleInfo(module3Handle)
			+ L"\n\n" + CurProcInfo();
		MessageBox(hw, data.c_str(), L"Информация о модулях", MB_OK);
	}

	// Информация о текущем процессе
	std::wstring GetModuleInfo() {
		WCHAR filePath[MAX_PATH];
		GetModuleFileName(NULL, filePath, sizeof(filePath));
		HMODULE moduleHandle = GetModuleHandle(NULL);
		return ModuleInfoToStr(filePath, moduleHandle);
	}

	// Информация о процессе по его имени
	std::wstring GetModuleInfo(LPCWSTR moduleName) {
		WCHAR filePath[MAX_PATH];
		HMODULE moduleHandle = GetModuleHandle(moduleName);
		GetModuleFileName(moduleHandle, filePath, sizeof(filePath));
		return ModuleInfoToStr(filePath, moduleHandle);
	}

	// Информация о процессе по его дескриптору
	std::wstring GetModuleInfo(HMODULE moduleHandle) {
		WCHAR filePath[MAX_PATH];
		GetModuleFileName(moduleHandle, filePath, sizeof(filePath));
		return ModuleInfoToStr(filePath, moduleHandle);
	}

	// Информация о процессе в виде строки
	std::wstring ModuleInfoToStr(LPCWSTR moduleFilePath, HMODULE moduleHandle) {
		std::wstring moduleFileName = std::wstring(moduleFilePath);
		moduleFileName = moduleFileName.substr(moduleFileName.find_last_of(L"/\\") + 1);
		std::wstring procInfo = L"Имя модуля: " + moduleFileName
			+ L"\nПолное имя модуля: " + std::wstring(moduleFilePath)
			+ L"\nДескриптора модуля: " + handleToStr(moduleHandle);
		return procInfo;
	}

	// Преобразовать адрес дескриптора в wide-строку
	std::wstring handleToStr(HANDLE _handle) {
		std::wstringstream stringStream;
		stringStream << _handle;
		return stringStream.str();
	}

	// Информация о процессе этой программы
	std::wstring CurProcInfo() {
		DWORD curProcID = GetCurrentProcessId();		// ID текущего процесса
		HANDLE curProcHandle = GetCurrentProcess();		// Псевдодескриптор текущего процесса
		HANDLE copyCurProcHandle;
		DuplicateHandle(curProcHandle, curProcHandle,	// Копия дескриптора текущего процесса
			curProcHandle, &copyCurProcHandle,
			PROCESS_ALL_ACCESS, FALSE, 0);
		HANDLE openCurProcHandle = OpenProcess(			// Открытый дескриптор текущего процесса
			PROCESS_ALL_ACCESS, FALSE, curProcID);

		std::wstring data = L"ID текущего процесса: " + std::to_wstring(curProcID)
			+ L"\nПсевдодескриптор текущего процесса: " + handleToStr(curProcHandle)
			+ L"\nРеальный дескриптор текущего процесса: " + handleToStr(copyCurProcHandle)
			+ L"\nНовый дескриптор текущего процесса: " + handleToStr(openCurProcHandle);

		CloseHandle(copyCurProcHandle);					// Закрываем копию дескриптора
		CloseHandle(openCurProcHandle);					// Закрываем открытый дескриптор
		return data;
	}

	// Вывести информацию о всех процессах
	void ShowListEnumAllProc(HWND hw, UINT procMax = 1000, BYTE modMax = 255, BYTE threadMax = 255) {
		PROCESSENTRY32W pe = { 0 };
		pe.dwSize = sizeof(pe);

		HANDLE tl32snap = INVALID_HANDLE_VALUE;
		tl32snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (tl32snap == INVALID_HANDLE_VALUE || !Process32First(tl32snap, &pe))
			MessageBox(hw, L"CreateToolhelp32Snapshot failed!", L"Список перечислений процессов", MB_OK);
		else {
			BYTE count = 0;
			do {
				if (pe.th32ProcessID == GetCurrentProcessId()) {
					std::wstringstream wStream;
					wStream << L"ID процесса: " << pe.th32ProcessID
						<< L"\nКол-во потоков: " << pe.cntThreads
						<< L"\nID родительского процесса: " << pe.th32ParentProcessID;
					ShowProcListModules(pe.th32ProcessID, wStream, modMax);
					ShowProcListThreads(pe.th32ProcessID, wStream, threadMax);
					count++;
					MessageBox(hw, wStream.str().c_str(), L"Список перечислений процессов", MB_OK);
				}
			} while (Process32Next(tl32snap, &pe) && count < procMax);
		}

		CloseHandle(tl32snap);
	}

	// Вывести информацию о модулях процесса
	void ShowProcListModules(DWORD ProcID, std::wstringstream& wStream, const BYTE modMax) {
		MODULEENTRY32 modEntry32 = { 0 };
		modEntry32.dwSize = sizeof(modEntry32);
		HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
		hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcID);
		if (hModuleSnap == INVALID_HANDLE_VALUE || !Module32First(hModuleSnap, &modEntry32)) {
			CloseHandle(hModuleSnap);
			wStream << L'\n';
			return;
		}
		else {

			BYTE count = 0;
			do {
				wStream << L"\nИмя модуля: " << modEntry32.szModule
					<< L"\nПуть к модулю: " << modEntry32.szExePath
					<< L"\nID процесса: " << modEntry32.th32ProcessID
					<< L"\nАдрес модуля: " << modEntry32.modBaseAddr
					<< L"\nРазмер модуля: " << modEntry32.modBaseSize;
				count++;
			} while (Module32Next(hModuleSnap, &modEntry32) && count < modMax);

			wStream << L'\n';
			CloseHandle(hModuleSnap);
		}
	}

	// Вывести информацию о потоках процесса
	void ShowProcListThreads(DWORD ProcID, std::wstringstream& wStream, const BYTE threadMax) {
		THREADENTRY32 te32 = { 0 };
		te32.dwSize = sizeof(te32);
		HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
		hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
		if (hThreadSnap == INVALID_HANDLE_VALUE || !Thread32First(hThreadSnap, &te32))
		{
			CloseHandle(hThreadSnap);
			wStream << L"\n\n";
			return;
		}
		else {

			BYTE count = 0;
			do {
				if (te32.th32OwnerProcessID == ProcID) {
					wStream << L"\nID потока: " << te32.th32ThreadID
					<< L"\nБазовый приоритет: " << te32.tpBasePri
					<< L"\nИзменение приоритета: " << te32.tpDeltaPri;
					count++;
				}
			} while (Thread32Next(hThreadSnap, &te32) && count < threadMax);

			wStream << L"\n\n";
			CloseHandle(hThreadSnap);
		}
	}

}

#endif
