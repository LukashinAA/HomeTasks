#include "matrix.h"
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cctype>

using namespace std;

const double matrix::EPS = 1e-9;

matrix::error::error(const char* msg, int line) : message_(nullptr) {
    char buffer[512];
    if (line != 0) {
        sprintf(buffer, "%s (line %d)", msg, line);
    } else {
        strcpy(buffer, msg);
    }
    message_ = new char[strlen(buffer) + 1];
    strcpy(message_, buffer);
}

matrix::error::error(const error& other) {
    if (other.message_) {
        message_ = new char[strlen(other.message_) + 1];
        strcpy(message_, other.message_);
    } else {
        message_ = nullptr;
    }
}

matrix::error& matrix::error::operator=(const error& other) {
    if (this != &other) {
        delete[] message_;
        if (other.message_) {
            message_ = new char[strlen(other.message_) + 1];
            strcpy(message_, other.message_);
        } else {
            message_ = nullptr;
        }
    }
    return *this;
}

matrix::error::~error() {
    delete[] message_;
}

const char* matrix::error::what() const {
    return message_ ? message_ : "Unknown error";
}

void matrix::_free() {
    if (data_) {
        for (int i = 0; i < rows_; ++i) {
            delete[] data_[i];
        }
        delete[] data_;
        data_ = nullptr;
    }
    rows_ = 0;
    cols_ = 0;
}

void matrix::_alloc(int r, int c) {
    if (r < 0 || c < 0) {
        throw error("Invalid matrix dimensions", __LINE__);
    }
    if (r == 0 || c == 0) {
        rows_ = 0;
        cols_ = 0;
        data_ = nullptr;
        return;
    }
    
    data_ = new double*[r];
    for (int i = 0; i < r; ++i) {
        data_[i] = new double[c];
        for (int j = 0; j < c; ++j) {
            data_[i][j] = 0.0;
        }
    }
    rows_ = r;
    cols_ = c;
}

void matrix::_copyFrom(const matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) return;
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            data_[i][j] = other.data_[i][j];
        }
    }
}

matrix::matrix() : data_(nullptr), rows_(0), cols_(0) {}

matrix::matrix(int n, int m) : data_(nullptr), rows_(0), cols_(0) {
    _alloc(n, m);
}

matrix::matrix(double val) : data_(nullptr), rows_(0), cols_(0) {
    _alloc(1, 1);
    data_[0][0] = val;
}

matrix::matrix(const double* arr, int m) : data_(nullptr), rows_(0), cols_(0) {
    if (!arr) throw error("Null array in row constructor", __LINE__);
    if (m <= 0) throw error("Invalid column count", __LINE__);
    _alloc(1, m);
    for (int j = 0; j < m; ++j) {
        data_[0][j] = arr[j];
    }
}

matrix::matrix(int n, const double* arr) : data_(nullptr), rows_(0), cols_(0) {
    if (!arr) throw error("Null array in column constructor", __LINE__);
    if (n <= 0) throw error("Invalid row count", __LINE__);
    _alloc(n, 1);
    for (int i = 0; i < n; ++i) {
        data_[i][0] = arr[i];
    }
}

matrix::matrix(const char* str) : data_(nullptr), rows_(0), cols_(0) {
    if (!str) throw error("Null string", __LINE__);
    
    // Просто собираем все числа из строки
    double numbers[1000];
    int numCount = 0;
    char numBuffer[100];
    int numPos = 0;
    bool inNumber = false;
    
    const char* p = str;
    while (*p) {
        if (isdigit(*p) || *p == '.' || *p == '-' || *p == '+') {
            if (!inNumber) {
                inNumber = true;
                numPos = 0;
            }
            numBuffer[numPos++] = *p;
        } else {
            if (inNumber) {
                numBuffer[numPos] = '\0';
                numbers[numCount++] = atof(numBuffer);
                inNumber = false;
            }
        }
        p++;
    }
    if (inNumber) {
        numBuffer[numPos] = '\0';
        numbers[numCount++] = atof(numBuffer);
    }
    
    if (numCount == 0) {
        throw error("No numbers found", __LINE__);
    }
    
    int nRows = 0;
    p = str;
    while (*p) {
        if (*p == '{' && *(p+1) != '{') {
            nRows++;
        }
        p++;
    }
    
    if (nRows == 0) {
        nRows = 1;
    }
    
    int nCols = numCount / nRows;
    
    if (nCols * nRows != numCount) {
        throw error("Invalid matrix dimensions", __LINE__);
    }
    
    _alloc(nRows, nCols);
    
    int idx = 0;
    for (int i = 0; i < nRows; ++i) {
        for (int j = 0; j < nCols; ++j) {
            data_[i][j] = numbers[idx++];
        }
    }
}

matrix::matrix(const matrix& other) : data_(nullptr), rows_(0), cols_(0) {
    _alloc(other.rows_, other.cols_);
    _copyFrom(other);
}

matrix::~matrix() {
    _free();
}

matrix& matrix::operator=(const matrix& other) {
    if (this != &other) {
        _free();
        _alloc(other.rows_, other.cols_);
        _copyFrom(other);
    }
    return *this;
}

matrix matrix::identity(int n) {
    if (n <= 0) throw error("Invalid size for identity matrix", __LINE__);
    matrix result(n, n);
    for (int i = 0; i < n; ++i) {
        result.data_[i][i] = 1.0;
    }
    return result;
}

matrix matrix::diagonal(const double* vals, int n) {
    if (!vals) throw error("Null pointer in diagonal", __LINE__);
    if (n <= 0) throw error("Invalid size for diagonal matrix", __LINE__);
    matrix result(n, n);
    for (int i = 0; i < n; ++i) {
        result.data_[i][i] = vals[i];
    }
    return result;
}

int matrix::rows() const {
    return rows_;
}

int matrix::columns() const {
    return cols_;
}

void matrix::set(int i, int j, double val) {
    if (i < 0 || i >= rows_ || j < 0 || j >= cols_) {
        throw error("Index out of bounds in set()", __LINE__);
    }
    data_[i][j] = val;
}

matrix matrix::operator[](int index) const {
    if (index >= 0 && index < rows_) {
        matrix result(1, cols_);
        for (int j = 0; j < cols_; ++j) {
            result.data_[0][j] = data_[index][j];
        }
        return result;
    }
    else if (index >= 0 && index < cols_) {
        matrix result(rows_, 1);
        for (int i = 0; i < rows_; ++i) {
            result.data_[i][0] = data_[i][index];
        }
        return result;
    }
    else {
        throw error("Index out of bounds: neither row nor column", __LINE__);
    }
}

matrix& matrix::operator*=(double scalar) {
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            data_[i][j] *= scalar;
        }
    }
    return *this;
}

matrix operator*(const matrix& lhs, double rhs) {
    matrix result(lhs);
    result *= rhs;
    return result;
}

matrix operator*(double lhs, const matrix& rhs) {
    return rhs * lhs;
}

matrix operator+(const matrix& lhs, const matrix& rhs) {
    if (lhs.rows_ != rhs.rows_ || lhs.cols_ != rhs.cols_) {
        throw matrix::error("Matrix dimensions must match for addition", __LINE__);
    }
    
    matrix result(lhs.rows_, lhs.cols_);
    
    for (int i = 0; i < lhs.rows_; ++i) {
        for (int j = 0; j < lhs.cols_; ++j) {
            result.data_[i][j] = lhs.data_[i][j] + rhs.data_[i][j];
        }
    }
    return result;
}

matrix& matrix::operator+=(const matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw error("Matrix dimensions must match for addition", __LINE__);
    }
    
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            data_[i][j] += other.data_[i][j];
        }
    }
    return *this;
}

matrix operator-(const matrix& lhs, const matrix& rhs) {
    if (lhs.rows_ != rhs.rows_ || lhs.cols_ != rhs.cols_) {
        throw matrix::error("Matrix dimensions must match for subtraction", __LINE__);
    }
    
    matrix result(lhs.rows_, lhs.cols_);
    
    for (int i = 0; i < lhs.rows_; ++i) {
        for (int j = 0; j < lhs.cols_; ++j) {
            result.data_[i][j] = lhs.data_[i][j] - rhs.data_[i][j];
        }
    }
    return result;
}

matrix& matrix::operator-=(const matrix& other) {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw error("Matrix dimensions must match for subtraction", __LINE__);
    }
    
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            data_[i][j] -= other.data_[i][j];
        }
    }
    return *this;
}

matrix operator*(const matrix& lhs, const matrix& rhs) {
    if (lhs.cols_ != rhs.rows_) {
        throw matrix::error("Incompatible dimensions for matrix multiplication", __LINE__);
    }
    
    matrix result(lhs.rows_, rhs.cols_);
    
    for (int i = 0; i < lhs.rows_; ++i) {
        for (int j = 0; j < rhs.cols_; ++j) {
            double sum = 0.0;
            for (int k = 0; k < lhs.cols_; ++k) {
                sum += lhs.data_[i][k] * rhs.data_[k][j];
            }
            result.data_[i][j] = sum;
        }
    }
    return result;
}

matrix& matrix::operator*=(const matrix& other) {
    *this = *this * other;
    return *this;
}

matrix matrix::operator-() const {
    matrix result(rows_, cols_);
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            result.data_[i][j] = -data_[i][j];
        }
    }
    return result;
}

bool matrix::operator==(const matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        return false;
    }
    
    for (int i = 0; i < rows_; ++i) {
        for (int j = 0; j < cols_; ++j) {
            if (fabs(data_[i][j] - other.data_[i][j]) > EPS) {
                return false;
            }
        }
    }
    return true;
}

bool matrix::operator!=(const matrix& other) const {
    return !(*this == other);
}

matrix operator|(const matrix& lhs, const matrix& rhs) {
    if (lhs.rows_ != rhs.rows_) {
        throw matrix::error("Number of rows must match for vertical concatenation", __LINE__);
    }
    
    matrix result(lhs.rows_, lhs.cols_ + rhs.cols_);
    
    for (int i = 0; i < lhs.rows_; ++i) {
        for (int j = 0; j < lhs.cols_; ++j) {
            result.data_[i][j] = lhs.data_[i][j];
        }
    }
    
    for (int i = 0; i < rhs.rows_; ++i) {
        for (int j = 0; j < rhs.cols_; ++j) {
            result.data_[i][lhs.cols_ + j] = rhs.data_[i][j];
        }
    }
    return result;
}

matrix operator/(const matrix& lhs, const matrix& rhs) {
    if (lhs.cols_ != rhs.cols_) {
        throw matrix::error("Number of columns must match for horizontal concatenation", __LINE__);
    }
    
    matrix result(lhs.rows_ + rhs.rows_, lhs.cols_);
    
    for (int i = 0; i < lhs.rows_; ++i) {
        for (int j = 0; j < lhs.cols_; ++j) {
            result.data_[i][j] = lhs.data_[i][j];
        }
    }
    
    for (int i = 0; i < rhs.rows_; ++i) {
        for (int j = 0; j < rhs.cols_; ++j) {
            result.data_[lhs.rows_ + i][j] = rhs.data_[i][j];
        }
    }
    return result;
}

ostream& operator<<(ostream& os, const matrix& mat) {
    for (int i = 0; i < mat.rows_; ++i) {
        os << "[";
        for (int j = 0; j < mat.cols_; ++j) {
            os << mat.data_[i][j];
            if (j < mat.cols_ - 1) {
                os << " ";
            }
        }
        os << "]";
        if (i < mat.rows_ - 1) {
            os << endl;
        }
    }
    return os;
}

double matrix::trace() const {
     if (rows_ != cols_) {
        throw error("Not square matrices", __LINE__);
    }
    
    double sum = 0.0;
    for (int i = 0; i < rows_; ++i) {
        sum += data_[i][i];
    }
    return sum;
}

bool matrix::operator^(const matrix& other) const {
    if (rows_ != cols_) {
        throw error("Left matrix must be square" , __LINE__);
    }
    if (other.rows_ != other.cols_) {
        throw error("Right matrix must be square" , __LINE__);
    }

    double trace1 = 0.0;
    double trace2 = 0.0;

    for (int i = 0; i < rows_; ++i) {
        trace1 += data_[i][i];
    }

    for (int i = 0; i < other.rows_; ++i) {
        trace2 += other.data_[i][i];
    }

    return trace1 >= trace2;
}


