#include <fstream>
#include <iostream>
#include <thread>
#include <string>
#include <windows.h>

using namespace std;

void CriticalSectionSync();
void PrintStrWithSync(LPCWSTR, CRITICAL_SECTION*);
void PrintStrWithoutSync(LPCWSTR);
void MutexSync();
void ShowFileMut(HANDLE, UINT*);
void EditFileMut(HANDLE, UINT*);
void EventSync();
void ThreadingWithEvent(HANDLE, LPCWSTR);

int main() {
	setlocale(LC_ALL, "");
	wcout << L"Лабораторная работа №3.\n\n";
	unsigned int task = 10;

	while (true) {
		wcout << L"Введите номер задания (1-3, 0 - выход):\n1. Сихронизация через критические секции.\n"
			<< L"2. Синхронизация через мьютексы.\n3. Синхронизация через события.\n\n";
		wcin >> task;
		if (task == 1) CriticalSectionSync();
		else if (task == 2) MutexSync();
		else if (task == 3) EventSync();
		else if (task == 0) {
			wcout << L"Выход из программы. Пока!\n";
			break;
		}
		else wcout << L"Неправильно введён номер задания, повторите попытку!\n\n";
	}
	system("pause");
	return 0;
}

// Синхронизация потоков через критические секции
void CriticalSectionSync() {
	// Объявление крит. секции и строк для вывода
	LPCRITICAL_SECTION cs = new CRITICAL_SECTION;
	LPCWSTR str1 = L"First Test String", str2 = L"Second Test String", str3 = L"Third Test String";

	// Использование крит. секции для синхронизации вывода строк
	wcout << L"Вывод строк с синхронизацией:\n";
	InitializeCriticalSection(cs);
	thread thr1(PrintStrWithSync, str1, cs);
	thread thr2(PrintStrWithSync, str2, cs);
	thread thr3(PrintStrWithSync, str3, cs);
	thr1.join();
	thr2.join();
	thr3.join();
	wcout << L'\n';
	DeleteCriticalSection(cs);
	delete cs;

	// Вывод строк без использования синхронизации
	wcout << L"Вывод строк без синхронизации:\n";
	thread thr4(PrintStrWithoutSync, str1);
	thread thr5(PrintStrWithoutSync, str2);
	thread thr6(PrintStrWithoutSync, str3);
	thr4.join();
	thr5.join();
	thr6.join();
	wcout << L'\n';
}

// Выводит посимвольно строку
void PrintStrWithSync(LPCWSTR str,  CRITICAL_SECTION* cs) {
	// С синхронизацией
	EnterCriticalSection(cs);
	for (int i = 0; i < lstrlen(str); i++) 
		wcout << str[i];
	wcout << L'\n';
	LeaveCriticalSection(cs);
}

// Выводит посимвольно строку
void PrintStrWithoutSync(LPCWSTR str) {
	// Без синхронизации
	for (int i = 0; i < lstrlen(str); i++) 
		wcout << str[i];
	wcout << L'\n';
}

// Синхронизация потоков через мьютекс
void MutexSync() {
	// Создаём мьютекс и строку для записи в файл
	HANDLE hMutex = CreateMutex(NULL, FALSE, L"MutexSync");
	wstring wstr = L"This string is cute";

	// Проверка на открытие приложения
	DWORD dwWaitResult = WaitForSingleObject(hMutex, 5000L);
	if (dwWaitResult == WAIT_OBJECT_0) {
		ReleaseMutex(hMutex);
	}
	else {
		wcout << L"Приложение уже было запущено!\nДанное приложение будет закрыто.\n";
		system("pause");
		exit(1);
	}

	// Перезаписываем информацию в файле
	wofstream fileWrite;
	locale defaultLocale("");
	fileWrite.imbue(defaultLocale);
	fileWrite.open("text.txt");
	fileWrite << wstr.c_str();
	fileWrite.close();

	// Создание потока для вывода информации из файла
	UINT num = wstr.length();
	thread thr1(ShowFileMut, hMutex, &num);
	thread thr2(EditFileMut, hMutex, &num);
	thr1.join();
	thr2.join();
	CloseHandle(hMutex);
	wcout << L'\n';
}

// Вывод файла в консоль
void ShowFileMut(HANDLE mutex, UINT* num) {
	std::wifstream fileRead;
	locale defaultLocale("");
	fileRead.imbue(defaultLocale);
	while ((*num) > 0) {
		DWORD dwWaitResult = WaitForSingleObject(mutex, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			fileRead.open("text.txt");
			wcout << fileRead.rdbuf() << L'\n';
			fileRead.close();
		}
		ReleaseMutex(mutex);
		Sleep(200);
	}
}

// Изменение файла
void EditFileMut(HANDLE mutex, UINT* num) {
	// Основной цикл процесса
	Sleep(10);
	while ((*num) > 0) {
		// Инициализация классов файлов и строк
		locale defaultLocale("");
		wifstream fileRead;
		fileRead.imbue(defaultLocale);
		wofstream fileWrite;
		fileWrite.imbue(defaultLocale);
		wstring wstr;

		// Ожидаем мьютекс
		DWORD dwWaitResult = WaitForSingleObject(mutex, INFINITE);
		if (dwWaitResult == WAIT_OBJECT_0) {
			// Читаем строку из файла
			fileRead.open("text.txt");
			getline(fileRead, wstr, L'\0');
			fileRead.close();

			// Обрезаем строку из файла
			wstr = wstr.substr(0, wstr.length() - 1);
			*num = wstr.length();

			// Записываем обрезанную строку в файл
			fileWrite.open("text.txt");
			fileWrite << wstr.c_str();
			fileWrite.close();
		}
		ReleaseMutex(mutex);
		Sleep(200);
	}
}

// Синхронизация через событие
void EventSync() {
	// Инициализация событий, активация события ev2
	HANDLE ev1 = CreateEvent(NULL, TRUE, FALSE, L"EventSync1");
	HANDLE ev2 = CreateEvent(NULL, TRUE, FALSE, L"EventSync2");
	SetEvent(ev2);

	// Создание потоков, передача им данных
	thread thr1(ThreadingWithEvent, ev1, L"Привет!\n");
	thread thr2(ThreadingWithEvent, ev1, L"Это текст.\n");
	thread thr3(ThreadingWithEvent, ev1, L"Другой текст.\n");
	thread thr4(ThreadingWithEvent, ev1, L"Ещё одна строка...\n");
	thread thr5(ThreadingWithEvent, ev1, L"Последняя строка!\n");
	thr1.join();
	thr2.join();
	thr3.join();
	thr4.join();
	thr5.join();

	// Ожидаем завершения всех потоков и закрываем события
	ResetEvent(ev1);
	CloseHandle(ev1);
	ResetEvent(ev2);
	CloseHandle(ev2);
	wcout << L'\n';
}

// Функция для каждого потока с использование событий
void ThreadingWithEvent(HANDLE ev, LPCWSTR str) {
	DWORD dwWaitResult = WaitForSingleObject(ev, INFINITE);
	if (dwWaitResult == WAIT_OBJECT_0) {
		wstring result = L"Поток №" + to_wstring(GetCurrentThreadId())
			+ L" получил строку: " + wstring(str);
		wcout << result;
	}
}
