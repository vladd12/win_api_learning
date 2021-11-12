#include <iostream>
#include <string>
#include <Windows.h>
#include <Shlwapi.h>
#include <Tlhelp32.h>

using namespace std;

// Объявления функций
void ShowModulesInfo();
void GetModuleInfo();
void GetModuleInfo(LPCWSTR);
void GetModuleInfo(HMODULE);
void ModuleInfoToConsole(LPCWSTR, HMODULE);
void ShowCurProcInfo();
void ShowListEnumAllProc(DWORD procMax = 1024, BYTE modMax = 255, BYTE threadMax = 255);
void ShowProcListModules(DWORD, const BYTE);
void ShowProcListThreads(DWORD, const BYTE);

// Главная функция для выполнения задачи лабораторной работы
int main() {
	setlocale(LC_ALL, "");
	wcout << L"Лабораторная работа №2.";
	unsigned int task = 10;

	while (true) {
		wcout << L"\n\nВведите номер задания (1-3, 0 - выход):\n1. Информация о процессе по имени или дескриптору.\n"
			<< L"2. Детальная информация о процессе.\n3. Список перечисления всех процессов, потоков, модулей и их свойств.\n\n";
		wcin >> task;
		if (task == 1) ShowModulesInfo();
		else if (task == 2) ShowCurProcInfo();
		else if (task == 3) ShowListEnumAllProc(1000, 200, 4);
		else if (task == 0) {
			wcout << L"Выход из программы. Пока!\n";
			break;
		}
		else wcout << L"Неправильно введён номер задания, повторите попытку!\n\n";
	}
	system("pause");
	return 0;
}

// Показать информацию о нужных модулях
void ShowModulesInfo() {
	LPWSTR module2Name = L"ntdll.dll";
	HMODULE module3Handle = GetModuleHandle(L"user32.dll");
	wcout << L"Информация о текущей программе:\n";
	GetModuleInfo();
	wcout << L"\n\nИнформация о модуле по имени модуля:\n";
	GetModuleInfo(module2Name);
	wcout << L"\n\nИнформация о модуле по дескриптору:\n";
	GetModuleInfo(module3Handle);
}

// Информация о текущем процессе
void GetModuleInfo() {
	WCHAR filePath[MAX_PATH];
	GetModuleFileName(NULL, filePath, sizeof(filePath));
	HMODULE moduleHandle = GetModuleHandle(NULL);
	return ModuleInfoToConsole(filePath, moduleHandle);
}

// Информация о процессе по его имени
void GetModuleInfo(LPCWSTR moduleName) {
	WCHAR filePath[MAX_PATH];
	HMODULE moduleHandle = GetModuleHandle(moduleName);
	GetModuleFileName(moduleHandle, filePath, sizeof(filePath));
	return ModuleInfoToConsole(filePath, moduleHandle);
}

// Информация о процессе по его дескриптору
void GetModuleInfo(HMODULE moduleHandle) {
	WCHAR filePath[MAX_PATH];
	GetModuleFileName(moduleHandle, filePath, sizeof(filePath));
	return ModuleInfoToConsole(filePath, moduleHandle);
}

// Информация о процессе в виде строки
void ModuleInfoToConsole(LPCWSTR moduleFilePath, HMODULE moduleHandle) {
	std::wstring moduleFileName = std::wstring(moduleFilePath);
	moduleFileName = moduleFileName.substr(moduleFileName.find_last_of(L"/\\") + 1);
	wcout << L"Имя модуля: " << moduleFileName.c_str()
		<< L"\nПолное имя модуля: " << moduleFilePath
		<< L"\nАдрес дескриптора модуля: " << moduleHandle;
}

// Информация о процессе этой программы
void ShowCurProcInfo() {
	DWORD curProcID = GetCurrentProcessId();		// ID текущего процесса
	HANDLE curProcHandle = GetCurrentProcess();		// Псевдодескриптор текущего процесса
	HANDLE copyCurProcHandle;
	DuplicateHandle(curProcHandle, curProcHandle,	// Реальный дескриптор текущего процесса
		curProcHandle, &copyCurProcHandle,
		PROCESS_ALL_ACCESS, FALSE, 0);
	HANDLE openCurProcHandle = OpenProcess(			// Новый дескриптор текущего процесса
		PROCESS_ALL_ACCESS, FALSE, curProcID);

	wcout << L"ID текущего процесса: " << curProcID
		<< L"\nПсевдодескриптор текущего процесса: " << curProcHandle
		<< L"\nРеальный дескриптор текущего процесса: " << copyCurProcHandle
		<< L"\nНовый дескриптор текущего процесса: " << openCurProcHandle;

	CloseHandle(copyCurProcHandle);
	CloseHandle(openCurProcHandle);
}

// Вывести информацию о всех процессах
void ShowListEnumAllProc(DWORD procMax, BYTE modMax, BYTE threadMax) {
	PROCESSENTRY32W pe = { 0 };
	pe.dwSize = sizeof(pe);

	HANDLE tl32snap = INVALID_HANDLE_VALUE;
	tl32snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (tl32snap == INVALID_HANDLE_VALUE || !Process32First(tl32snap, &pe))
		wcout << L"CreateToolhelp32Snapshot failed!\n\n";
	else {
		DWORD count = 0;
		do {
			wcout << L"ID процесса: " << pe.th32ProcessID
				<< L"\nКол-во потоков: " << pe.cntThreads
				<< L"\nID родительского процесса: " << pe.th32ParentProcessID;
			ShowProcListModules(pe.th32ProcessID, modMax);
			ShowProcListThreads(pe.th32ProcessID, threadMax);

		} while (Process32Next(tl32snap, &pe) && count < procMax);
	}
	CloseHandle(tl32snap);
}

// Вывести информацию о модулях процесса
void ShowProcListModules(DWORD ProcID, const BYTE modMax) {
	MODULEENTRY32 modEntry32 = { 0 };
	modEntry32.dwSize = sizeof(modEntry32);
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ProcID);
	if (hModuleSnap == INVALID_HANDLE_VALUE || !Module32First(hModuleSnap, &modEntry32)) {
		CloseHandle(hModuleSnap);
		wcout << L'\n';
		return;
	}
	else {

		BYTE count = 0;
		do {
			wcout << L"\nИмя модуля: " << modEntry32.szModule
				<< L"\nПуть к модулю: " << modEntry32.szExePath
				<< L"\nID процесса: " << modEntry32.th32ProcessID
				<< L"\nАдрес модуля: " << modEntry32.modBaseAddr
				<< L"\nРазмер модуля: " << modEntry32.modBaseSize;
			count++;
		} while (Module32Next(hModuleSnap, &modEntry32) && count < modMax);

		wcout << L'\n';
		CloseHandle(hModuleSnap);
	}
}

// Вывести информацию о потоках процесса
void ShowProcListThreads(DWORD ProcID, const BYTE threadMax) {
	THREADENTRY32 te32 = { 0 };
	te32.dwSize = sizeof(te32);
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE || !Thread32First(hThreadSnap, &te32))
	{
		CloseHandle(hThreadSnap);
		wcout << L"\n\n";
		return;
	}
	else {

		BYTE count = 0;
		do {
			if (te32.th32OwnerProcessID == ProcID) {
				wcout << L"\nID потока: " << te32.th32ThreadID
					<< L"\nБазовый приоритет: " << te32.tpBasePri
					<< L"\nИзменение приоритета: " << te32.tpDeltaPri;
				count++;
			}
		} while (Thread32Next(hThreadSnap, &te32) && count < threadMax);

		wcout << L"\n\n";
		CloseHandle(hThreadSnap);
	}
}