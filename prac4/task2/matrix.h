#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

class matrix {
private:
    double** data_;
    int rows_;
    int cols_;

    void _free();
    void _alloc(int r, int c);
    void _copyFrom(const matrix& other);

public:
    class error {
    private:
        char* message_;
    public:
        error(const char* msg, int line = 0);
        error(const error& other);
        error& operator=(const error& other);
        ~error();
        const char* what() const;
    };

    static const double EPS;

    matrix();
    matrix(int n, int m);
    explicit matrix(double val);
    matrix(const double* arr, int m);
    matrix(int n, const double* arr);
    explicit matrix(const char* str);

    matrix(const matrix& other);
    ~matrix();
    matrix& operator=(const matrix& other);

    static matrix identity(int n);
    static matrix diagonal(const double* vals, int n);

    int rows() const;
    int columns() const;

    void set(int i, int j, double val);

    matrix operator[](int index) const;

    matrix& operator*=(double scalar);
    friend matrix operator*(const matrix& lhs, double rhs);
    friend matrix operator*(double lhs, const matrix& rhs);

    friend matrix operator+(const matrix& lhs, const matrix& rhs);
    friend matrix operator-(const matrix& lhs, const matrix& rhs);
    friend matrix operator*(const matrix& lhs, const matrix& rhs);

    matrix& operator+=(const matrix& other);
    matrix& operator-=(const matrix& other);
    matrix& operator*=(const matrix& other);

    matrix operator-() const;

    bool operator==(const matrix& other) const;
    bool operator!=(const matrix& other) const;

    friend matrix operator|(const matrix& lhs, const matrix& rhs);
    friend matrix operator/(const matrix& lhs, const matrix& rhs);

    friend std::ostream& operator<<(std::ostream& os, const matrix& mat);
};

#endif
