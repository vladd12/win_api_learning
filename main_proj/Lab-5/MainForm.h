#pragma once

#include <Windows.h>
#include <Tlhelp32.h>
#include <sstream>
#include <iomanip>

namespace Lab5 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainForm : public System::Windows::Forms::Form {
	public:
		// Конструктор
		MainForm(void) {
			InitializeComponent();
		}

	protected:
		// Деструктор
		~MainForm() {
			if (components) delete components;
		}

	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart2;
	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart3;
	private: System::Windows::Forms::Label^  lblOutput;
	private: System::Windows::Forms::Button^  btnAction;
	private: System::Windows::Forms::Button^  btnProcTree;
	private: System::Windows::Forms::RichTextBox^  rTxtBoxOutput;
	private: System::Windows::Forms::Button^  btnProcList;
	private: System::Windows::Forms::TextBox^  txtBoxProcID;
	private: System::Windows::Forms::Label^  lblProcID;
	private: System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint1 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0,
				0));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint2 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0,
				0));
			System::Windows::Forms::DataVisualization::Charting::Title^  title1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea2 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint3 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0,
				0));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint4 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0,
				0));
			System::Windows::Forms::DataVisualization::Charting::Title^  title2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea3 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::Windows::Forms::DataVisualization::Charting::Series^  series3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Series());
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint5 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0,
				0));
			System::Windows::Forms::DataVisualization::Charting::DataPoint^  dataPoint6 = (gcnew System::Windows::Forms::DataVisualization::Charting::DataPoint(0,
				0));
			System::Windows::Forms::DataVisualization::Charting::Title^  title3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Title());
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart2 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->chart3 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->lblOutput = (gcnew System::Windows::Forms::Label());
			this->btnAction = (gcnew System::Windows::Forms::Button());
			this->btnProcTree = (gcnew System::Windows::Forms::Button());
			this->rTxtBoxOutput = (gcnew System::Windows::Forms::RichTextBox());
			this->btnProcList = (gcnew System::Windows::Forms::Button());
			this->txtBoxProcID = (gcnew System::Windows::Forms::TextBox());
			this->lblProcID = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart3))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(12, 12);
			this->chart1->Name = L"chart1";
			this->chart1->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Berry;
			series1->ChartArea = L"ChartArea1";
			series1->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Pie;
			series1->Legend = L"Legend1";
			series1->Name = L"PhysMemory";
			dataPoint1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			dataPoint1->LegendText = L"Использовано";
			dataPoint2->LegendText = L"Доступно";
			series1->Points->Add(dataPoint1);
			series1->Points->Add(dataPoint2);
			this->chart1->Series->Add(series1);
			this->chart1->Size = System::Drawing::Size(300, 260);
			this->chart1->TabIndex = 0;
			this->chart1->Text = L"chartPhysMem";
			title1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title1->Name = L"PhysMemory";
			title1->Text = L"Физическая память";
			this->chart1->Titles->Add(title1);
			// 
			// chart2
			// 
			chartArea2->Name = L"ChartArea1";
			this->chart2->ChartAreas->Add(chartArea2);
			legend2->Name = L"Legend1";
			this->chart2->Legends->Add(legend2);
			this->chart2->Location = System::Drawing::Point(12, 278);
			this->chart2->Name = L"chart2";
			this->chart2->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Berry;
			series2->ChartArea = L"ChartArea1";
			series2->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Pie;
			series2->Legend = L"Legend1";
			series2->Name = L"PageMemory";
			dataPoint3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			dataPoint3->LegendText = L"Использовано";
			dataPoint4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			dataPoint4->LegendText = L"Доступно";
			series2->Points->Add(dataPoint3);
			series2->Points->Add(dataPoint4);
			this->chart2->Series->Add(series2);
			this->chart2->Size = System::Drawing::Size(300, 260);
			this->chart2->TabIndex = 1;
			this->chart2->Text = L"chartPhysMem";
			title2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title2->Name = L"PageMemory";
			title2->Text = L"Страничные блоки памяти";
			this->chart2->Titles->Add(title2);
			// 
			// chart3
			// 
			chartArea3->Name = L"ChartArea1";
			this->chart3->ChartAreas->Add(chartArea3);
			legend3->Name = L"Legend1";
			this->chart3->Legends->Add(legend3);
			this->chart3->Location = System::Drawing::Point(12, 544);
			this->chart3->Name = L"chart3";
			this->chart3->Palette = System::Windows::Forms::DataVisualization::Charting::ChartColorPalette::Berry;
			series3->ChartArea = L"ChartArea1";
			series3->ChartType = System::Windows::Forms::DataVisualization::Charting::SeriesChartType::Pie;
			series3->Legend = L"Legend1";
			series3->Name = L"VirtualMemory";
			dataPoint5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			dataPoint5->LegendText = L"Использовано";
			dataPoint6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			dataPoint6->LegendText = L"Доступно";
			series3->Points->Add(dataPoint5);
			series3->Points->Add(dataPoint6);
			this->chart3->Series->Add(series3);
			this->chart3->Size = System::Drawing::Size(300, 260);
			this->chart3->TabIndex = 2;
			this->chart3->Text = L"chartPhysMem";
			title3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.8F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			title3->Name = L"VirtualMemory";
			title3->Text = L"Виртуальная память";
			this->chart3->Titles->Add(title3);
			// 
			// lblOutput
			// 
			this->lblOutput->AutoSize = true;
			this->lblOutput->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lblOutput->Location = System::Drawing::Point(318, 72);
			this->lblOutput->Name = L"lblOutput";
			this->lblOutput->Size = System::Drawing::Size(200, 20);
			this->lblOutput->TabIndex = 3;
			this->lblOutput->Text = L"Информация о памяти";
			// 
			// btnAction
			// 
			this->btnAction->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnAction->Location = System::Drawing::Point(318, 12);
			this->btnAction->Name = L"btnAction";
			this->btnAction->Size = System::Drawing::Size(282, 57);
			this->btnAction->TabIndex = 4;
			this->btnAction->Text = L"Информация о памяти";
			this->btnAction->UseVisualStyleBackColor = true;
			this->btnAction->Click += gcnew System::EventHandler(this, &MainForm::btnAction_Click);
			// 
			// btnProcTree
			// 
			this->btnProcTree->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnProcTree->Location = System::Drawing::Point(318, 747);
			this->btnProcTree->Name = L"btnProcTree";
			this->btnProcTree->Size = System::Drawing::Size(282, 57);
			this->btnProcTree->TabIndex = 5;
			this->btnProcTree->Text = L"Построить дерево процесса";
			this->btnProcTree->UseVisualStyleBackColor = true;
			this->btnProcTree->Click += gcnew System::EventHandler(this, &MainForm::btnProcTree_Click);
			// 
			// rTxtBoxOutput
			// 
			this->rTxtBoxOutput->Font = (gcnew System::Drawing::Font(L"Courier New", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->rTxtBoxOutput->Location = System::Drawing::Point(606, 12);
			this->rTxtBoxOutput->Name = L"rTxtBoxOutput";
			this->rTxtBoxOutput->ReadOnly = true;
			this->rTxtBoxOutput->Size = System::Drawing::Size(547, 792);
			this->rTxtBoxOutput->TabIndex = 6;
			this->rTxtBoxOutput->Text = L"";
			// 
			// btnProcList
			// 
			this->btnProcList->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnProcList->Location = System::Drawing::Point(318, 544);
			this->btnProcList->Name = L"btnProcList";
			this->btnProcList->Size = System::Drawing::Size(282, 57);
			this->btnProcList->TabIndex = 7;
			this->btnProcList->Text = L"Список процессов";
			this->btnProcList->UseVisualStyleBackColor = true;
			this->btnProcList->Click += gcnew System::EventHandler(this, &MainForm::btnProcList_Click);
			// 
			// txtBoxProcID
			// 
			this->txtBoxProcID->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->txtBoxProcID->Location = System::Drawing::Point(318, 714);
			this->txtBoxProcID->Name = L"txtBoxProcID";
			this->txtBoxProcID->Size = System::Drawing::Size(282, 27);
			this->txtBoxProcID->TabIndex = 8;
			// 
			// lblProcID
			// 
			this->lblProcID->AutoSize = true;
			this->lblProcID->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->lblProcID->Location = System::Drawing::Point(318, 691);
			this->lblProcID->Name = L"lblProcID";
			this->lblProcID->Size = System::Drawing::Size(193, 20);
			this->lblProcID->TabIndex = 9;
			this->lblProcID->Text = L"Введите ID процесса:";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1165, 822);
			this->Controls->Add(this->lblProcID);
			this->Controls->Add(this->txtBoxProcID);
			this->Controls->Add(this->btnProcList);
			this->Controls->Add(this->rTxtBoxOutput);
			this->Controls->Add(this->btnProcTree);
			this->Controls->Add(this->btnAction);
			this->Controls->Add(this->lblOutput);
			this->Controls->Add(this->chart3);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->chart1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Лаб. работа №5";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		// Метод вызывается при нажатии на кнопку "Инфомрация о памяти"
		void btnAction_Click(System::Object^  sender, System::EventArgs^  e) {
			// Получение данных о состоянии памяти
			MEMORYSTATUSEX memStat = { 0 };
			memStat.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memStat);

			// Вывод в текстовом виде
			const DWORDLONG divider = 1024 * 1024;
			lblOutput->Text = L"Физическая память.\nВсего: " + memStat.ullTotalPhys / divider + L" МБ.\n"
				+ L"Использовано: " + (memStat.ullTotalPhys - memStat.ullAvailPhys) / divider + L" МБ.\n"
				+ L"Доступно: " + memStat.ullAvailPhys / divider + L" МБ.\n\n"
				+ L"Страничные блоки памяти.\nВсего: " + memStat.ullTotalPageFile / divider + L" МБ.\n"
				+ L"Использовано: " + (memStat.ullTotalPageFile - memStat.ullAvailPageFile) / divider + L" МБ.\n"
				+ L"Доступно: " + memStat.ullAvailPageFile / divider + L" МБ.\n\n"
				+ L"Виртуальная память.\nВсего: " + memStat.ullTotalVirtual / divider + L" МБ.\n"
				+ L"Использовано: " + (memStat.ullTotalVirtual - memStat.ullAvailVirtual) / divider + L" МБ.\n"
				+ L"Доступно: " + memStat.ullAvailVirtual / divider + L" МБ.";

			// Вывод в графическом виде
			chart1->Series["PhysMemory"]->Points->Clear();
			chart2->Series["PageMemory"]->Points->Clear();
			chart3->Series["VirtualMemory"]->Points->Clear();

			chart1->Series["PhysMemory"]->Points->AddY(memStat.ullTotalPhys - memStat.ullAvailPhys);
			chart1->Series["PhysMemory"]->Points->AddY(memStat.ullAvailPhys);
			chart1->Series["PhysMemory"]->Points[0]->LegendText = L"Использовано";
			chart1->Series["PhysMemory"]->Points[1]->LegendText = L"Доступно";

			chart2->Series["PageMemory"]->Points->AddY(memStat.ullTotalPageFile - memStat.ullAvailPageFile);
			chart2->Series["PageMemory"]->Points->AddY(memStat.ullAvailPageFile);
			chart2->Series["PageMemory"]->Points[0]->LegendText = L"Использовано";
			chart2->Series["PageMemory"]->Points[1]->LegendText = L"Доступно";

			chart3->Series["VirtualMemory"]->Points->AddY(memStat.ullTotalVirtual - memStat.ullAvailVirtual);
			chart3->Series["VirtualMemory"]->Points->AddY(memStat.ullAvailVirtual);
			chart3->Series["VirtualMemory"]->Points[0]->LegendText = L"Использовано";
			chart3->Series["VirtualMemory"]->Points[1]->LegendText = L"Доступно";
		}

		// Метод вызывается при нажатии на кнопку "Список процессов"
		void btnProcList_Click(System::Object^  sender, System::EventArgs^  e) {
			// Объявление констант и создание снепшота
			const int idField = 6, procNameField = 35;
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 pstruct = { 0 };
			pstruct.dwSize = sizeof(pstruct);
			Process32First(hSnapshot, &pstruct);

			// Форматированный вывод в richTextBox
			std::wstringstream sStream;
			sStream << std::setw(idField) << L"ID" << L" | " << std::setw(procNameField) << L"Имя процесса" << L'\n';
			sStream << std::setfill(L'_') << std::setw(idField) << L"_" << L" | " << std::setw(procNameField) << L'\n' << std::setfill(L' ');
			rTxtBoxOutput->Text = gcnew String(sStream.str().c_str());
			sStream.str(L"");

			// Вывод информации о процессах из снапшота в richTextBox
			do {
				sStream << std::setw(idField) << pstruct.th32ProcessID << L" | " << std::setw(procNameField) << pstruct.szExeFile << L'\n';
				rTxtBoxOutput->Text = rTxtBoxOutput->Text + gcnew String(sStream.str().c_str());
				sStream.str(L"");
			} while (Process32Next(hSnapshot, &pstruct));
		}

		// Создание карты памяти процесса
		void getMapProcess(DWORD id) {
			// Инициализируем начальные значения
			HANDLE hHandle = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, id);
			MEMORY_BASIC_INFORMATION mbi;
			std::wstringstream wsStream;
			rTxtBoxOutput->Text = L"";

			// Циклично обходим выделенные страницы виртуальной памяти
			DWORD finAddress = 0x7FFFFFFF;
			for (DWORD bAddress = 0; bAddress < finAddress; bAddress += mbi.RegionSize) {
				VirtualQueryEx(hHandle, (void*)bAddress, &mbi, sizeof(mbi));
				wsStream << std::hex << L"\nCurrent address: 0x" << bAddress << std::dec << L"\nBase address: 0x"
					<< mbi.BaseAddress << L"\nArea size: " << mbi.RegionSize / 1024 << L" KB.";
				if (mbi.State != MEM_FREE) wsStream << L"\nAllocation base address: 0x" << mbi.AllocationBase;

				// Узнаём состояние памяти
				switch (mbi.State) {
				case MEM_FREE:
					wsStream << L"\nState: Free\n";
					break;
				case MEM_RESERVE:
					wsStream << L"\nState: Reserve\nType: ";
					switch (mbi.Type) {
					case MEM_IMAGE:
						wsStream << L"Image";
						break;
					case MEM_MAPPED:
						wsStream << L"Mapped";
						break;
					case MEM_PRIVATE:
						wsStream << L"Private";
						break;
					default:
						wsStream << L"Unknown";
						break;
					}
					wsStream << L'\n';
					break;
				case MEM_COMMIT:
					wsStream << L"\nState: Commit\nType: ";
					switch (mbi.Type) {
					case MEM_IMAGE:
						wsStream << L"Image";
						break;
					case MEM_MAPPED:
						wsStream << L"Mapped";
						break;
					case MEM_PRIVATE:
						wsStream << L"Private";
						break;
					default:
						wsStream << L"Unknown";
						break;
					}
					wsStream << L"\nProtect: ";
					switch (mbi.Protect) {
					case PAGE_EXECUTE:
						wsStream << L"Execute";
						break;
					case PAGE_NOACCESS:
						wsStream << L"No access";
						break;
					case PAGE_READONLY:
						wsStream << L"Read only";
						break;
					case PAGE_READWRITE:
						wsStream << L"Read/write";
						break;
					case PAGE_WRITECOPY:
						wsStream << L"Writecopy";
						break;
					default:
						wsStream << L"Unknown";
						break;
					}
					wsStream << L'\n';
					break;
				default:
					wsStream << "\nState: Unknown\n";
					break;
				}

				// Вывод в текстовое поле
				rTxtBoxOutput->Text = rTxtBoxOutput->Text + gcnew String(wsStream.str().c_str());
				wsStream.str(L"");
			}
		}

		// Метод вызывается при нажатии на кнопку "Построить дерево процесса"
		void btnProcTree_Click(System::Object^  sender, System::EventArgs^  e) {
			// Проверки
			if (txtBoxProcID->Text == L"") {
				MessageBox::Show(this, "Введите ID процесса в текстовое поле!", "Ошибка!",
				MessageBoxButtons::OK, MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
				return;
			}
			System::UInt32 ProcID;
			try {
				ProcID = Convert::ToUInt32(txtBoxProcID->Text);
			}
			catch (System::FormatException^ e) {
				MessageBox::Show(this, "Введите корректные данные в текстовое поле!", "Ошибка!",
				MessageBoxButtons::OK, MessageBoxIcon::Error, MessageBoxDefaultButton::Button1);
				return;
			}

			// После прохождения всех проверок
			getMapProcess(ProcID);
		}

	};
}
