#pragma once

#include <Windows.h>

namespace LabOS_5_1 {

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
			this->chart1->Size = System::Drawing::Size(325, 305);
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
			this->chart2->Location = System::Drawing::Point(343, 12);
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
			this->chart2->Size = System::Drawing::Size(325, 305);
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
			this->chart3->Location = System::Drawing::Point(674, 12);
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
			this->chart3->Size = System::Drawing::Size(325, 305);
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
			this->lblOutput->Location = System::Drawing::Point(12, 323);
			this->lblOutput->Name = L"lblOutput";
			this->lblOutput->Size = System::Drawing::Size(56, 20);
			this->lblOutput->TabIndex = 3;
			this->lblOutput->Text = L"Empty";
			// 
			// btnAction
			// 
			this->btnAction->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->btnAction->Location = System::Drawing::Point(674, 323);
			this->btnAction->Name = L"btnAction";
			this->btnAction->Size = System::Drawing::Size(326, 309);
			this->btnAction->TabIndex = 4;
			this->btnAction->Text = L"Информация о памяти";
			this->btnAction->UseVisualStyleBackColor = true;
			this->btnAction->Click += gcnew System::EventHandler(this, &MainForm::btnAction_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1012, 644);
			this->Controls->Add(this->btnAction);
			this->Controls->Add(this->lblOutput);
			this->Controls->Add(this->chart3);
			this->Controls->Add(this->chart2);
			this->Controls->Add(this->chart1);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"MainForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Имя формы";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->chart3))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private:
		// Метод вызывается при нажатии на кнопку
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
	};
}
