#include <iostream>
#include <string>
#include <windows.h>
#include "MailslotFuncs.hpp"

double CalcDet(double*);

// Глобальные переменные
LPWSTR ClientSlotName = L"\\\\.\\mailslot\\Client";
HANDLE evClientRead = NULL, evServerRead = NULL;

// Основная функция программы
int wmain(int argc, WCHAR* argv[]) {
	// Создать события
	evClientRead = CreateEvent(NULL, FALSE, FALSE, L"ClientRead");
	evServerRead = CreateEvent(NULL, FALSE, FALSE, L"ServerRead");

	// Создать mailslot и активировать событие, начать чтение mailslot
	LPWSTR ServerSlotName = argv[0];
	std::wcout << argv[0] << L'\n';
	HANDLE hSlot = MakeMailslot(ServerSlotName);
	SetEvent(evServerRead);
	Sleep(500);
	double* data = new double[4];
	ReadServerSlot(hSlot, data);
	ResetEvent(evServerRead);

	// Вычисление определителя матрицы 2х2
	// и отправка результата клиенту
	double result = CalcDet(data);
	WriteClientSlot(ClientSlotName, &result);
	SetEvent(evClientRead);

	// Очищаем память, закрываем дескрипторы
	CloseHandle(hSlot);
	delete data;
	return 0;
}

// Функция подсчёта определителя матрицы 2х2
double CalcDet(double* inVal) {
	double result = inVal[0] * inVal[3] - inVal[1] * inVal[2];
	std::wcout << result << std::endl;
	return result;
}