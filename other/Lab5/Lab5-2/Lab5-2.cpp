#include <iostream>
#include <windows.h>
#include <TlHelp32.h>

using namespace std;

void ShowProcList();
void ShowMemoryMapProc();

int main() {
	setlocale(LC_ALL, "");
	wcout << L"Лабораторная работа №5.\n\n";
	unsigned int task = 10;

	while (true) {
		wcout << L"Введите номер задания (1-2, 0 - выход):\n"
			<< L"1. Вывести имена всех процессов и их ID.\n"
			<< L"2. Вывести карту виртуальной памяти процесса по его ID.\n\n";
		wcin >> task;
		if (task == 1) ShowProcList();
		else if (task == 2) ShowMemoryMapProc();
		else if (task == 0) {
			wcout << L"Выход из программы. Пока!\n";
			break;
		}
		else wcout << L"Неправильно введён номер задания, повторите попытку!\n\n";
	}
	system("pause");
	return 0;
}

// Вывод списка процессов
void ShowProcList() {
	// Создание снимка процессов
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pstruct = { 0 };
	pstruct.dwSize = sizeof(pstruct);
	Process32First(hSnapshot, &pstruct);

	// Форматированный вывод информации о процессах из снимка
	wcout << L"Формат вывода: <ID процесса>: <имя процесса>\n";
	do {
		wcout << pstruct.th32ProcessID << L": " << pstruct.szExeFile << L'\n';
	} while (Process32Next(hSnapshot, &pstruct));
	wcout << L'\n';
}

// Вывод карты виртуальной памяти процесса по его ID
void ShowMemoryMapProc() {
	DWORD ProcID;
	wcout << L"Введите ID процесса: ";
	wcin >> ProcID;

	// Инициализируем начальные значения
	HANDLE hHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ProcID);
	MEMORY_BASIC_INFORMATION mbi;

	// Циклично обходим выделенные страницы виртуальной памяти
	DWORD finAddress = 0x7FFFFFFF;
	for (DWORD bAddress = 0; bAddress < finAddress; bAddress += mbi.RegionSize) {
		VirtualQueryEx(hHandle, (void*)bAddress, &mbi, sizeof(mbi));
		wcout << std::hex << L"\nCurrent address: 0x" << bAddress << std::dec << L"\nBase address: 0x"
			<< mbi.BaseAddress << L"\nRegion size: " << mbi.RegionSize / 1024 << L" KB.";
		if (mbi.State != MEM_FREE) wcout << L"\nAllocation base address: 0x" << mbi.AllocationBase;

		// Узнаём состояние памяти
		switch (mbi.State) {
		case MEM_FREE:
			wcout << L"\nState: Free\n";
			break;
		case MEM_RESERVE:
			wcout << L"\nState: Reserve\nType: ";
			switch (mbi.Type) {
			case MEM_IMAGE:
				wcout << L"Image";
				break;
			case MEM_MAPPED:
				wcout << L"Mapped";
				break;
			case MEM_PRIVATE:
				wcout << L"Private";
				break;
			default:
				wcout << L"Unknown";
				break;
			}
			wcout << L'\n';
			break;
		case MEM_COMMIT:
			wcout << L"\nState: Commit\nType: ";
			switch (mbi.Type) {
			case MEM_IMAGE:
				wcout << L"Image";
				break;
			case MEM_MAPPED:
				wcout << L"Mapped";
				break;
			case MEM_PRIVATE:
				wcout << L"Private";
				break;
			default:
				wcout << L"Unknown";
				break;
			}
			wcout << L"\nProtect: ";
			switch (mbi.Protect) {
			case PAGE_EXECUTE:
				wcout << L"Execute";
				break;
			case PAGE_NOACCESS:
				wcout << L"No access";
				break;
			case PAGE_READONLY:
				wcout << L"Read only";
				break;
			case PAGE_READWRITE:
				wcout << L"Read/write";
				break;
			case PAGE_WRITECOPY:
				wcout << L"Writecopy";
				break;
			default:
				wcout << L"Unknown";
				break;
			}
			wcout << L'\n';
			break;
		default:
			wcout << "\nState: Unknown\n";
			break;
		}
	}
	wcout << L'\n';
}