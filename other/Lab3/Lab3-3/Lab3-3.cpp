#include <iostream>
#include <windows.h>

using namespace std;

int main() {
	setlocale(LC_ALL, "");
	wcout << L"Синхронизаци¤ через события.\n\n";
	HANDLE ev1 = CreateEvent(NULL, TRUE, FALSE, L"EventSync1");
	HANDLE ev2 = CreateEvent(NULL, TRUE, FALSE, L"EventSync2");
	// Ожидание события, которое активирует другое событие
	while (true) {
		DWORD dwWaitResult = WaitForSingleObject(ev2, 5000L);
		if (dwWaitResult == WAIT_OBJECT_0) {
			wcout << L"Событие активировано!\n";
			SetEvent(ev1);
			break;
		}
		else {
			wcout << L"Ожидание событи¤...\n";
		}
	}
	// Очистка данных
	ResetEvent(ev2);
	CloseHandle(ev2);
	system("pause");
	return 0;
}