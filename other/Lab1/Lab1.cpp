#include <iostream>
#include <Windows.h>
#include <lmcons.h>
#include <sysinfoapi.h>

using namespace std;

// Объявления функций
void ShowCompNameAndUserName();
void SystemDirs();
void OSVer();
void SystemMetrics();
const LPCWSTR IntToBoolString(int);
void SystemPrms();
void SystemColors();
void SystemTime();
void ShowTime(SYSTEMTIME&);
void OtherFuncs();

int main()
{
	setlocale(LC_ALL, "");
	wcout << L"Лабораторная работа №1.\n\n";
	unsigned int task = 10;

	while (true) {
		wcout << L"Введите номер задания (1-8, 0 - выход):\n1. Показать имя компьютера и текущего пользователя.\n"
			<< L"2. Вывести пути к системным директориям.\n3. Вывести версию операционной системы.\n"
			<< L"4. Вывести системные метрики.\n5. Вывести системные параметры.\n6. Изменение системных цветов.\n"
			<< L"7. Вывести системное время.\n8. Работа с другими функциями API Windows.\n\n";
		wcin >> task;
		if (task == 1) ShowCompNameAndUserName();
		else if (task == 2) SystemDirs();
		else if (task == 3) OSVer();
		else if (task == 4) SystemMetrics();
		else if (task == 5) SystemPrms();
		else if (task == 6) SystemColors();
		else if (task == 7) SystemTime();
		else if (task == 8) OtherFuncs();
		else if (task == 0) {
			wcout << L"Выход из программы. Пока!\n";
			break;
		}
		else wcout << L"Неправильно введён номер задания, повторите попытку!\n\n";
	}
	system("pause");
	return 0;
}


// Функция для получения имени компьютера и текущего пользователя
void ShowCompNameAndUserName() {
	// Получение имени компьютера
	DWORD sizeBuffer = UNLEN;
	LPWSTR stringBuffer = new WCHAR[sizeBuffer + 1];
	GetComputerName(stringBuffer, &sizeBuffer);
	wcout << L"Имя компьютера: " << stringBuffer << L'\n';

	// Вывод имени пользователя
	GetUserName(stringBuffer, &sizeBuffer);
	wcout << L"Имя пользователя: " << stringBuffer << L"\n\n";
	delete[] stringBuffer;
}

// Функция для получения путей к системным каталогам
void SystemDirs() {
	LPWSTR path = new WCHAR[MAX_PATH];
	const UINT bfPathSize = sizeof(WCHAR) * MAX_PATH;

	GetSystemDirectory(path, bfPathSize);
	wcout << L"Системный каталог: " << path << L'\n';
	GetWindowsDirectory(path, bfPathSize);
	wcout << L"Каталог Windows: " << path << L'\n';
	GetTempPath(MAX_PATH, path);
	wcout << L"Каталог временных файлов: " << path << L"\n\n";
	delete[] path;
}

// Функция для получения версии ОС
void OSVer() {
	NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
	OSVERSIONINFOEXW osVerInfo;
	*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

	if (NULL != RtlGetVersion)
	{
		osVerInfo.dwOSVersionInfoSize = sizeof(osVerInfo);
		RtlGetVersion(&osVerInfo);
	}

	wcout << L"Версия ОС: " << osVerInfo.dwMajorVersion << L'.' << osVerInfo.dwMinorVersion << L'\n'
		<< L"Версия сборки: " << osVerInfo.dwBuildNumber << L'\n'
		<< L"Платформа ОС: " << osVerInfo.dwPlatformId << L"\n\n";
}

// Вывод системных метрик
void SystemMetrics() {
	int result = GetSystemMetrics(SM_CMOUSEBUTTONS);
	wcout << L"Количество клавиш мыши: " << result << L'\n';
	result = GetSystemMetrics(SM_MOUSEWHEELPRESENT);
	wcout << L"Мышь имеет колесо прокрутки: " << IntToBoolString(result) << L'\n';
	result = GetSystemMetrics(SM_CXSCREEN);
	wcout << L"Разрешение экрана: " << result;
	result = GetSystemMetrics(SM_CYSCREEN);
	wcout << L" x " << result << L"\n\n";
}

// Вспомогательный метод для отображения
// bool из int в виде wchar_t строк
const LPCWSTR IntToBoolString(int num) {
	if (num == 0) return L"нет";
	else return L"да";
}

// Вывод системных параметров
void SystemPrms() {
	BOOL param;
	ACCESSTIMEOUT acTOData = { 0 };
	acTOData.cbSize = sizeof(ACCESSTIMEOUT);
	ANIMATIONINFO  animInfoData = { 0 };
	animInfoData.cbSize = sizeof(ANIMATIONINFO);

	SystemParametersInfo(SPI_GETBEEP, 0, &param, 0);
	wcout << L"Предупреждающий звуковой сигнал включён: " << IntToBoolString(param) << L'\n';

	SystemParametersInfo(SPI_GETCLEARTYPE, 0, &param, 0);
	wcout << L"Функция сглаживания текста на мониторе включена: "
		<< IntToBoolString(param) << L'\n';

	SystemParametersInfo(SPI_GETANIMATION, animInfoData.cbSize, &animInfoData, 0);
	wcout << L"Анимации сворачивания и закрытия окон включена: "
		<< IntToBoolString(animInfoData.iMinAnimate) << L"\n\n";
}

// Вывод информации о системных цветах
void SystemColors() {
	int displayElements[] = { COLOR_BTNFACE, COLOR_WINDOWTEXT, COLOR_WINDOWFRAME };

	COLORREF color1 = GetSysColor(displayElements[0]);  // Цвет кнопки
	COLORREF color2 = GetSysColor(displayElements[1]);  // Цвет текста
	COLORREF color3 = GetSysColor(displayElements[2]);  // Цвет рамки кнопки
	COLORREF oldColors[] = { color1, color2, color3 };

	COLORREF newColors[] = {
		RGB(170, 5, 240),	// Фиолетовый
		RGB(255, 0, 247),	// Сиреневый
		RGB(0, 255, 0)		// Зелёный
	};

	SetSysColors(3, displayElements, newColors);
	wcout << L"Изменены цвета элементов.\n";
	MessageBox(NULL, L"Изменены цвета элементов.\n", L"Системные цвета", MB_OK);
	SetSysColors(3, displayElements, oldColors);
}

// Вывод информации о локальном и системном времени
void SystemTime() {
	SYSTEMTIME var1;
	GetLocalTime(&var1);
	wcout << L"Локальное время:\n";
	ShowTime(var1);
	GetSystemTime(&var1);
	wcout << L"Системное время:\n";
	ShowTime(var1);
}

// Вывод информации о системном времени
void ShowTime(SYSTEMTIME& sysTime) {
	wcout << L"Дата: " << sysTime.wDay << L'.' << sysTime.wMonth
		<< L'.' << sysTime.wYear << L"\nВремя: " << sysTime.wHour << L':' << sysTime.wMinute
		<< L':' << sysTime.wSecond << L':' << sysTime.wMilliseconds << L"\n\n";
}

// Дополнительные функции
void OtherFuncs() {
	// Возвращает локаль в виде <язык>-<РЕГИОН> согласно RFC 4646
	UINT localNameSize = LOCALE_NAME_MAX_LENGTH;
	LPWSTR localNamePtr = new WCHAR[localNameSize];
	GetSystemDefaultLocaleName(localNamePtr, localNameSize);
	wcout << L"Системная локаль: " << localNamePtr << L'\n';
	delete[] localNamePtr;

	// Время в миллисекундах с запуска операционной системы
	wcout << L"Время с запуска системы (мс): " << GetTickCount() << L'\n';

	// Выводит данный процесс в виде командной строки, так же может выводить
	// аргументы, с которыми вызвана программа
	wcout << L"Данный процесс в виде команды командной строки: " << GetCommandLine() << L'\n';

	// Вызываем звук ошибки: MB_OK, MB_ICONERROR, 0xFFFFFFFF
	MessageBeep(MB_ICONERROR);
	wcout << L"Звук ошибки при появлении этого окна.\n\n";
}