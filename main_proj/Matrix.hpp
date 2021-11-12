/*----------Matrix.hpp----------*/
#ifndef MATRIX_H
#define MATRIX_H

#include <random>
#include <string>
#include <sstream>

// Класс для описания матрицы
class Matrix {
private:
	double* _arr = 0;

	// Функция получения случайного числа в заданном диапазоне
	int getRandNum(int start = -10, int end = 10) {
		// Источник энтропии.
		std::random_device random_device;
		// Генератор случайных чисел
		std::mt19937 generator(random_device());
		// Равномерное распределение на участке [start, end]
		std::uniform_int_distribution<> distribution(start, end);
		// Получаем случайное число
		int x = distribution(generator);
		return x;
	}

	// Функция генерации матрицы
	void generateMatrix() {
		matrix_size = rows * cols;
		_arr = new double[matrix_size];
		for (size_t i = 0; i < matrix_size; i++) {
			_arr[i] = static_cast<double>(getRandNum());
		}
	}

public:
	// Кол-во строк и столбцов
	unsigned int rows, cols, matrix_size = 0;

	// Конструктор по умолчанию
	explicit Matrix() {
		rows = 3;
		cols = 3;
		generateMatrix();
	}

	// Конструктор с параметрами
	explicit Matrix(const unsigned int &rows, const unsigned int &cols) {
		this->rows = rows;
		this->cols = cols;
		generateMatrix();
	}

	// Конструктор копирования
	Matrix(const Matrix &rhs) {
		if (matrix_size != 0) delete[] _arr;
		matrix_size = rhs.matrix_size;
		_arr = new double[matrix_size];
		for (size_t i = 0; i < matrix_size; i++)
			_arr[i] = rhs._arr[i];
		rows = rhs.rows;
		cols = rhs.cols;
	}

	// Деструктор
	~Matrix() {
		delete[] _arr;
	}

	// Возвращает ячейку матрицы
	const double GetCell(const unsigned int &row, const unsigned int &col) {
		return _arr[col + row * cols];
	}

	// Устанавливает значение для ячейки матрицы
	void SetCell(const unsigned int &row, const unsigned int &col, const double &val) {
		_arr[col + row * cols] = val;
	}

	// Создать новую матрицу, исключив строки и столбцы
	Matrix Exclude(Matrix& mat, const unsigned int &row, const unsigned int &col) {
		Matrix result(mat.rows - 1, mat.cols - 1);
		unsigned int index = 0;
		for (size_t i = 0; i < mat.matrix_size; i++) {
			unsigned int curRow = i / mat.cols;
			unsigned int curCol = i % mat.cols;
			if (curRow != row && curCol != col) {
				result._arr[index] = mat._arr[i];
				index++;
			}
		}
		return result;
	}

	// Строковое представление матрицы
	std::wstring ToWString(Matrix& mat) {
		std::wstringstream stringStream;
		for (size_t i = 0; i < mat.matrix_size; i++) {
			if (i != 0 && (i % mat.cols == 0)) stringStream << L'\n';
			stringStream << mat._arr[i] << L' ';
		}
		return stringStream.str();
	}

};

#endif