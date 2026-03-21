# Matrix Class - Вариант 4.2 (Вещественная матрица)

## Описание проекта

Реализация абстрактного типа данных (АТД) "Вещественная матрица" на языке C++.
Класс `matrix` предоставляет набор операций для работы с матрицами вещественных чисел (тип `double`).

Проект выполнен в рамках учебного задания и соответствует требованиям варианта 4.2, включая все операции из варианта 4.1 и дополнительные операции.

## Структура проекта
├── matrix.h # Заголовочный файл с объявлением класса
├── matrix.cpp # Реализация методов класса
├── main.cpp # Тестовая программа
├── Makefile # Файл сборки
└── README.md # Документация

text

## Требования к компиляции

- Компилятор: g++ (поддерживающий стандарт C++11)
- Операционная система: Linux / macOS / Windows (с MinGW или WSL)

## Сборка и запуск

### Компиляция и запуск тестов

```bash
make
или

bash
make test
Очистка объектных файлов и исполняемого файла

bash
make clean
Ручная компиляция

bash
g++ -Wall -Wextra -std=c++11 -g -c matrix.cpp
g++ -Wall -Wextra -std=c++11 -g -c main.cpp
g++ -Wall -Wextra -std=c++11 -g -o matrix_test matrix.o main.o
./matrix_test
Реализованные операции (Вариант 4.1)

№	Операция	Описание
1	matrix(int n, int m)	Конструктор матрицы n×m со значениями 0.0
2	matrix(double val)	Конструктор матрицы 1×1 с элементом val
3	matrix(const double* arr, int m)	Конструктор строки из массива длины m
4	matrix(int n, const double* arr)	Конструктор столбца из массива длины n
5	matrix(const char* str)	Конструктор из строкового представления
6	static matrix identity(int n)	Возвращает единичную матрицу n×n
7	static matrix diagonal(const double* vals, int n)	Возвращает диагональную матрицу
8	int rows() const	Возвращает количество строк
9	int columns() const	Возвращает количество столбцов
10	void set(int i, int j, double val)	Устанавливает значение элемента
11	matrix operator[](int i) const	Возвращает i-ю строку или i-й столбец
12	matrix& operator*=(double scalar)	Умножение на скаляр с присваиванием
13	matrix operator*(const matrix& lhs, double rhs)	Умножение матрицы на скаляр
14	matrix operator*(double lhs, const matrix& rhs)	Умножение скаляра на матрицу
15	friend ostream& operator<<	Вывод матрицы в поток
Дополнительные операции (Вариант 4.2)

№	Операция	Описание
16	matrix operator+(const matrix& lhs, const matrix& rhs)	Поэлементное сложение матриц
17	matrix& operator+=(const matrix& other)	Сложение с присваиванием
18	matrix operator-(const matrix& lhs, const matrix& rhs)	Поэлементное вычитание матриц
19	matrix& operator-=(const matrix& other)	Вычитание с присваиванием
20	matrix operator*(const matrix& lhs, const matrix& rhs)	Матричное умножение
21	matrix& operator*=(const matrix& other)	Матричное умножение с присваиванием
22	matrix operator-() const	Унарный минус (смена знака всех элементов)
23	bool operator==(const matrix& other) const	Сравнение матриц с точностью EPS
24	bool operator!=(const matrix& other) const	Отрицание равенства
25	matrix operator|(const matrix& lhs, const matrix& rhs)	Вертикальная конкатенация (справа)
26	matrix operator/(const matrix& lhs, const matrix& rhs)	Горизонтальная конкатенация (снизу)
Вложенный класс исключений

Класс matrix::error используется для обработки ошибок:

cpp
try {
    matrix m(1000, 1000);
} catch (const matrix::error& e) {
    cerr << "Error: " << e.what() << endl;
}
Статическая константа точности

cpp
static const double EPS = 1e-9;
Используется в операторах сравнения для корректного сравнения чисел с плавающей точкой.

Примеры использования

Создание матриц

cpp
// Матрица 2x3, заполненная нулями
matrix A(2, 3);

// Единичная матрица 3x3
matrix I = matrix::identity(3);

// Диагональная матрица
double vals[] = {1.5, 2.5, 3.5};
matrix D = matrix::diagonal(vals, 3);

// Матрица из строки
matrix M("{{1,2,3},{4,5,6}}");
Доступ к элементам

cpp
matrix A(2, 2);
A.set(0, 0, 1.0);
A.set(0, 1, 2.0);
A.set(1, 0, 3.0);
A.set(1, 1, 4.0);

// Получение строки
matrix row = A[0];   // [1 2]

// Получение столбца
matrix col = A[1];   // [2 4] (если индекс 1 не является строкой)
Арифметические операции

cpp
matrix A(2, 2), B(2, 2);
// ... заполнение матриц ...

matrix C = A + B;      // Поэлементное сложение
matrix D = A - B;      // Поэлементное вычитание
matrix E = A * B;      // Матричное умножение
matrix F = -A;         // Унарный минус
matrix G = A * 2.5;    // Умножение на скаляр
Сравнение матриц

cpp
if (A == B) {
    cout << "Matrices are equal" << endl;
}

if (A != B) {
    cout << "Matrices are different" << endl;
}
Конкатенация

cpp
matrix A(2, 2), B(2, 2);

// Вертикальная конкатенация (A справа от B)
matrix vert = A | B;   // результат: 2x4

// Горизонтальная конкатенация (A снизу от B)
matrix hor = A / B;    // результат: 4x2
Вывод матрицы

cpp
matrix A(2, 3);
cout << A << endl;
Вывод:

text
[1 2 3]
[4 5 6]
Проверка утечек памяти

На Linux (valgrind)

bash
valgrind --leak-check=full --show-leak-kinds=all ./matrix_test
На macOS (leaks)

bash
leaks --atExit -- ./matrix_test
Особенности реализации

Инкапсуляция: Все поля класса приватные, доступ через публичные методы
Динамическая память: Используется двойной указатель double** data_ для хранения матрицы
Правило трех: Реализованы конструктор копирования, оператор присваивания и деструктор
Константные методы: Методы, не изменяющие объект, объявлены как const
Обработка ошибок: Встроенный класс исключений с информацией о строке ошибки
Без STL: Не используются контейнеры стандартной библиотеки
