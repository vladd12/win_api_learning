/*----------Lab1.hpp----------*/
#ifndef LAB1
#define LAB1

#include <Windows.h>
#include <string>
#include <lmcons.h>
#include <sysinfoapi.h>

namespace lab1 {

	// Объявления функций
	void Lab1Exec();
	void ShowNameComputerAndUser(std::wstring& defStr);
	void SysDirectories(std::wstring& defStr);
	void OSVersion(std::wstring& defStr);
	void SysMetrics(std::wstring& defStr);
	WCHAR* IntToBoolString(int num);
	void SysParams(std::wstring& defStr);
	void SysColors(std::wstring& defStr);
	void SysTime(std::wstring& defStr);
	void OtherWinFunsc(std::wstring& defStr);
	std::wstring PowerStatsToWString(const SYSTEM_POWER_STATUS& sPwStats);

	// Функция для выполнения всех функций лабораторной работы
	void Lab1Exec() {
		std::wstring defString;
		ShowNameComputerAndUser(defString);
		SysDirectories(defString);
		OSVersion(defString);
		SysMetrics(defString);
		SysParams(defString);
		SysColors(defString);
		SysTime(defString);
		OtherWinFunsc(defString);
	}

	// Функция для получения имени компьютера и текущего пользователя
	void ShowNameComputerAndUser(std::wstring& defStr) {
		// Получение имени компьютера
		DWORD sizeBuffer = UNLEN;
		LPWSTR stringBuffer = new WCHAR[sizeBuffer + 1];
		GetComputerName(stringBuffer, &sizeBuffer);
		defStr = L"Имя компьютера: " + std::wstring(stringBuffer) + L'\n';

		// Вывод имени пользователя
		GetUserName(stringBuffer, &sizeBuffer);
		defStr = defStr + L"Имя пользователя: " + std::wstring(stringBuffer) + L'\n';
		MessageBox(NULL, defStr.c_str(), L"Имя компьютера и пользователя", MB_OK);
		delete[] stringBuffer;
	}

	// Функция для получения путей к системным каталогам
	void SysDirectories(std::wstring& defStr) {
		LPWSTR path = new WCHAR[MAX_PATH];
		const UINT bfPathSize = sizeof(WCHAR) * MAX_PATH;

		GetSystemDirectory(path, bfPathSize);
		defStr = L"Системный каталог: " + std::wstring(path) + L'\n';
		GetWindowsDirectory(path, bfPathSize);
		defStr = defStr + L"Каталог Windows: " + std::wstring(path) + L'\n';
		GetTempPath(MAX_PATH, path);
		defStr = defStr + L"Каталог временных файлов: " + std::wstring(path) + L'\n';
		MessageBox(NULL, defStr.c_str(), L"Системные каталоги", MB_OK);
		delete[] path;
	}

	// Функция для получения версии ОС
	void OSVersion(std::wstring& defStr) {
		NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
		OSVERSIONINFOEXW osInfo;
		*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandle(L"ntdll"), "RtlGetVersion");

		if (NULL != RtlGetVersion)
		{
			osInfo.dwOSVersionInfoSize = sizeof(osInfo);
			RtlGetVersion(&osInfo);
		}

		defStr = L"Версия ОС: " + std::to_wstring(osInfo.dwMajorVersion)
			+ L'.' + std::to_wstring(osInfo.dwMinorVersion) + L'\n'
			+ L"Версия сборки: " + std::to_wstring(osInfo.dwBuildNumber) + L'\n'
			+ L"Платформа ОС: " + std::to_wstring(osInfo.dwPlatformId);
		MessageBox(NULL, defStr.c_str(), L"Версия ОС", MB_OK);
	}

	// Вывод системных метрик
	void SysMetrics(std::wstring& defStr) {
		int result = GetSystemMetrics(SM_CMOUSEBUTTONS);
		defStr = L"Количество клавиш мыши: " + std::to_wstring(result) + L'\n';
		result = GetSystemMetrics(SM_MOUSEWHEELPRESENT);
		defStr = defStr + L"Мышь имеет колесо прокрутки: " + IntToBoolString(result) + L'\n';
		result = GetSystemMetrics(SM_SWAPBUTTON);
		defStr = defStr + L"Мышь для левши: " + IntToBoolString(result) + L'\n';
		result = GetSystemMetrics(SM_CXSCREEN);
		defStr = defStr + L"Разрешение экрана: " + std::to_wstring(result);
		result = GetSystemMetrics(SM_CYSCREEN);
		defStr = defStr + L'x' + std::to_wstring(result) + L'\n';
		result = GetSystemMetrics(SM_CXFULLSCREEN);
		defStr = defStr + L"Ширина окна: " + std::to_wstring(result) + L'\n';
		result = GetSystemMetrics(SM_CYFULLSCREEN);
		defStr = defStr + L"Высота окна: " + std::to_wstring(result) + L'\n';
		MessageBox(NULL, defStr.c_str(), L"Метрики", MB_OK);
	}

	// Вспомогательный метод для отображения
	// bool из int в виде wchar_t строк
	WCHAR* IntToBoolString(int num) {
		if (num == 0) return L"нет";
		else return L"да";
	}

	// Вывод системных параметров
	void SysParams(std::wstring& defStr) {
		RECT rect;
		UINT param;
		ACCESSTIMEOUT acTOData = { 0 };
		acTOData.cbSize = sizeof(ACCESSTIMEOUT);
		ICONMETRICS icoMetrData = { 0 };
		icoMetrData.cbSize = sizeof(ICONMETRICS);
		ANIMATIONINFO  animInfoData = { 0 };
		animInfoData.cbSize = sizeof(ANIMATIONINFO);

		SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
		defStr = L"Рабочая зона: " + std::to_wstring(rect.right - rect.left)
			+ L'x' + std::to_wstring(rect.bottom - rect.top) + L'\n';

		SystemParametersInfo(SPI_GETBEEP, 0, &param, 0);
		defStr = defStr + L"Предупреждающий звуковой сигнал включён: "
			+ IntToBoolString(param) + L'\n';

		SystemParametersInfo(SPI_GETACCESSTIMEOUT, sizeof(ACCESSTIMEOUT), &acTOData, 0);
		defStr = defStr + L"Период ожидания функций специальных возможностей (с): "
			+ std::to_wstring(acTOData.iTimeOutMSec / 1000) + L'\n';

		SystemParametersInfo(SPI_GETANIMATION, sizeof(ANIMATIONINFO), &animInfoData, 0);
		defStr = defStr + L"Анимации сворачивания и закрытия окон включена: "
			+ IntToBoolString(animInfoData.iMinAnimate) + L'\n';

		SystemParametersInfo(SPI_GETICONMETRICS, sizeof(ICONMETRICS), &icoMetrData, 0);
		defStr = defStr + L"Размер иконок: " + std::to_wstring(icoMetrData.iHorzSpacing)
			+ L'x' + std::to_wstring(icoMetrData.iVertSpacing) + L'\n'
			+ L"Заголовки иконок переносятся на новую строку: "
			+ IntToBoolString(icoMetrData.iTitleWrap) + L'\n'
			+ L"Шрифт, используемый у иконок: " + icoMetrData.lfFont.lfFaceName + L'\n';

		MessageBox(NULL, defStr.c_str(), L"Системные параметры", MB_OK);
	}

	// Вывод информации о системных цветах
	void SysColors(std::wstring& defStr) {
		int displayElements[] = { COLOR_BTNFACE, COLOR_WINDOWTEXT, COLOR_WINDOWFRAME };

		COLORREF color1 = GetSysColor(displayElements[0]);  // Цвет кнопки
		COLORREF color2 = GetSysColor(displayElements[1]);  // Цвет текста
		COLORREF color3 = GetSysColor(displayElements[2]);  // Цвет рамки кнопки
		COLORREF oldColors[] = { color1, color2, color3 };

		COLORREF newColors[] = {
			RGB(0, 255, 0),		// Зелёный
			RGB(0, 0, 255),		// Синий
			RGB(255, 0, 0)		// Красный
		};

		SetSysColors(3, displayElements, newColors);
		defStr = L"Изменены цвета элементов.\n";
		MessageBox(NULL, defStr.c_str(), L"Системные цвета", MB_OK);
		SetSysColors(3, displayElements, oldColors);
	}

	// Вывод информации о локальном и системном времени
	void SysTime(std::wstring& defStr) {
		SYSTEMTIME var1, var2;
		GetLocalTime(&var1);
		GetSystemTime(&var2);

		defStr = L"Локальное время:\nдата: " + std::to_wstring(var1.wDay) + L'.'
			+ std::to_wstring(var1.wMonth) + L'.' + std::to_wstring(var1.wYear)
			+ L"\nвремя: " + std::to_wstring(var1.wHour) + L':'
			+ std::to_wstring(var1.wMinute) + L':' + std::to_wstring(var1.wSecond)
			+ L':' + std::to_wstring(var1.wMilliseconds) + L"\n\n";

		defStr = defStr + L"Системное время:\nдата: " + std::to_wstring(var2.wDay) + L'.'
			+ std::to_wstring(var2.wMonth) + L'.' + std::to_wstring(var2.wYear)
			+ L"\nвремя: " + std::to_wstring(var2.wHour) + L':'
			+ std::to_wstring(var2.wMinute) + L':' + std::to_wstring(var2.wSecond)
			+ L':' + std::to_wstring(var2.wMilliseconds) + L'\n';

		MessageBox(NULL, defStr.c_str(), L"Информация о времени", MB_OK);
	}

	// Дополнительные функции
	void OtherWinFunsc(std::wstring& defStr) {
		// Получение информации о курсоре
		CURSORINFO curInf = { 0 };
		curInf.cbSize = sizeof(CURSORINFO);
		GetCursorInfo(&curInf);
		defStr = L"Позиция курсора:\nX: " + std::to_wstring(curInf.ptScreenPos.x)
			+ L"   Y: " + std::to_wstring(curInf.ptScreenPos.y) + L"\n\n";

		// Вызываем звук ошибки: MB_OK, MB_ICONERROR, 0xFFFFFFFF
		MessageBeep(MB_ICONERROR);
		defStr = defStr + L"Звук ошибки при появлении этого окна.\n\n";

		// Узнать идентификатор текущей ACP в системе
		UINT curACP = GetACP();
		defStr = defStr + L"ID текущей страницы кодировки ANSI: "
			+ std::to_wstring(curACP) + L"\n\n";

		// Состояние питания компьютера
		SYSTEM_POWER_STATUS sysPwStats = { 0 };
		GetSystemPowerStatus(&sysPwStats);
		defStr = defStr + PowerStatsToWString(sysPwStats);

		MessageBox(NULL, defStr.c_str(), L"Доп. функции", MB_OK);
	}

	// Функция для получения из структуры текстовой информации о состоянии батареи
	std::wstring PowerStatsToWString(const SYSTEM_POWER_STATUS& sPwStats) {
		std::wstring str = L"Состояние питания компьютера.\n";

		// Тип тока источника
		str = str + L"Тип тока: ";
		switch (sPwStats.ACLineStatus) {
		case 0:
			str = str + L"постоянный ток (direct current / DC)";
			break;
		case 1:
			str = str + L"переменный ток (alternating current/ AC)";
			break;
		case 255:
			str = str + L"Unknown status";
			break;
		}

		// Процент энергии батареи
		str = str + L"\nПроцент энергии батареи: ";
		if (sPwStats.BatteryLifePercent == 255)
			str = str + L"Unknown status";
		else str = str + std::to_wstring(sPwStats.BatteryLifePercent);

		// Оставшееся время работы батареи
		str = str + L"\nОставшееся время работы батареи (мин): ";
		if (sPwStats.BatteryLifeTime == -1)
			str = str + L"батарея отсутствует или заряжается";
		else str = str + std::to_wstring(sPwStats.BatteryLifeTime / 60);

		// Полное время работы батареи
		str = str + L"\nПолное время работы батареи (мин): ";
		if (sPwStats.BatteryFullLifeTime == -1)
			str = str + L"батарея отсутствует или заряжается\n";
		else str = str + std::to_wstring(sPwStats.BatteryFullLifeTime / 60) + L'\n';

		return str;
	}

}

#endif