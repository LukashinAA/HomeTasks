#include "matrix.h"
#include <iostream>

using namespace std;

int main() {
    cout << "=== TESTING VARIANT 4.2 ===" << endl;
    
    try {
        cout << "\n--- Test 1: Constructors ---" << endl;
        
        matrix m1;
        cout << "m1 (empty): " << m1.rows() << "x" << m1.columns() << endl;
        
        matrix m2(2, 3);
        cout << "m2 (2x3 zeros):\n" << m2 << endl;
        
        matrix m3(5.5);
        cout << "m3 (5.5):\n" << m3 << endl;
        
        double arr[] = {1, 2, 3, 4};
        matrix m4(arr, 4);
        cout << "m4 (row from array):\n" << m4 << endl;
        
        matrix m5(4, arr);
        cout << "m5 (column from array):\n" << m5 << endl;
        
        cout << "\n--- Test 2: identity and diagonal ---" << endl;
        
        matrix I = matrix::identity(3);
        cout << "identity(3):\n" << I << endl;
        
        double diagVals[] = {1.1, 2.2, 3.3};
        matrix D = matrix::diagonal(diagVals, 3);
        cout << "diagonal({1.1,2.2,3.3}, 3):\n" << D << endl;
        
        cout << "\n--- Test 3: set() and operator[] ---" << endl;
        
        matrix A(2, 3);
        A.set(0, 0, 1); A.set(0, 1, 2); A.set(0, 2, 3);
        A.set(1, 0, 4); A.set(1, 1, 5); A.set(1, 2, 6);
        cout << "A after set:\n" << A << endl;
        
        matrix row0 = A[0];
        cout << "A[0] (first row):\n" << row0 << endl;
        
        matrix col2 = A[2];
        cout << "A[2] (third column):\n" << col2 << endl;
        
        cout << "\n--- Test 4: Scalar multiplication ---" << endl;
        
        matrix B = A * 2.5;
        cout << "A * 2.5:\n" << B << endl;
        
        matrix C = 3.0 * A;
        cout << "3.0 * A:\n" << C << endl;
        
        cout << "\n--- Test 5: matrix(char*) constructor ---" << endl;
        
        matrix strMat("{{1,2,3},{4,5,6}}");
        cout << "From string '{{1,2,3},{4,5,6}}':\n" << strMat << endl;
        
        cout << "\n=== TESTING VARIANT 4.2 OPERATIONS ===\n" << endl;
        
        matrix X(2, 2);
        X.set(0, 0, 1); X.set(0, 1, 2);
        X.set(1, 0, 3); X.set(1, 1, 4);
        
        matrix Y(2, 2);
        Y.set(0, 0, 5); Y.set(0, 1, 6);
        Y.set(1, 0, 7); Y.set(1, 1, 8);
        
        cout << "X:\n" << X << endl;
        cout << "Y:\n" << Y << endl;
        
        matrix sum = X + Y;
        cout << "X + Y:\n" << sum << endl;
        
        matrix X_copy = X;
        X_copy += Y;
        cout << "X += Y:\n" << X_copy << endl;
        
        matrix diff = X - Y;
        cout << "X - Y:\n" << diff << endl;
        
        X_copy = X;
        X_copy -= Y;
        cout << "X -= Y:\n" << X_copy << endl;
        
        matrix M1(2, 3);
        M1.set(0, 0, 1); M1.set(0, 1, 2); M1.set(0, 2, 3);
        M1.set(1, 0, 4); M1.set(1, 1, 5); M1.set(1, 2, 6);
        
        matrix M2(3, 2);
        M2.set(0, 0, 7); M2.set(0, 1, 8);
        M2.set(1, 0, 9); M2.set(1, 1, 10);
        M2.set(2, 0, 11); M2.set(2, 1, 12);
        
        cout << "\nM1 (2x3):\n" << M1 << endl;
        cout << "M2 (3x2):\n" << M2 << endl;
        
        matrix prod = M1 * M2;
        cout << "M1 * M2 (matrix multiplication):\n" << prod << endl;
        
        matrix X2(2, 2);
        X2.set(0, 0, 1); X2.set(0, 1, 2);
        X2.set(1, 0, 3); X2.set(1, 1, 4);
        
        matrix neg = -X2;
        cout << "\n-X:\n" << neg << endl;
        
        matrix P(2, 2);
        P.set(0, 0, 1); P.set(0, 1, 2);
        P.set(1, 0, 3); P.set(1, 1, 4);
        
        matrix Q(2, 2);
        Q.set(0, 0, 1); Q.set(0, 1, 2);
        Q.set(1, 0, 3); Q.set(1, 1, 4);
        
        cout << "\nP == Q: " << (P == Q ? "true" : "false") << endl;
        cout << "P != Q: " << (P != Q ? "true" : "false") << endl;
        
        matrix L(2, 2);
        L.set(0, 0, 1); L.set(0, 1, 2);
        L.set(1, 0, 3); L.set(1, 1, 4);
        
        matrix R(2, 2);
        R.set(0, 0, 5); R.set(0, 1, 6);
        R.set(1, 0, 7); R.set(1, 1, 8);
        
        matrix vert = L | R;
        cout << "\nL | R (vertical concatenation):\n" << vert << endl;
        
        matrix hor = L / R;
        cout << "L / R (horizontal concatenation):\n" << hor << endl;
        
        cout << "\n=== ALL TESTS PASSED ===" << endl;
        matrix q1(3, 3);
        q1.set(0, 0, 1); q1.set(0, 1, 2); q1.set(0, 2, 3);
        q1.set(1, 0, 4); q1.set(1, 1, 5); q1.set(1, 2, 6);
        q1.set(2, 0, 7); q1.set(2, 1, 8); q1.set(2, 2, 9);
        
        matrix q2(3, 3);
        q2.set(0, 0, 9); q2.set(0, 1, 8); q2.set(0, 2, 7);
        q2.set(1, 0, 6); q2.set(1, 1, 5); q2.set(1, 2, 4);
        q2.set(2, 0, 3); q2.set(2, 1, 2); q2.set(2, 2, 1);
        
        matrix q3(3, 3);
        q3.set(0, 0, 10); q3.set(0, 1, 0); q3.set(0, 2, 0);
        q3.set(1, 0, 0); q3.set(1, 1, 20); q3.set(1, 2, 0);
        q3.set(2, 0, 0); q3.set(2, 1, 0); q3.set(2, 2, 30);
        
        cout << "\nMatrix m1:\n" << q1 << endl;
        cout << "Trace of m1 = " << q1.trace() << endl;
        
        cout << "\nMatrix m2:\n" << q2 << endl;
        cout << "Trace of m2 = " << q2.trace() << endl;
        
        cout << "\nMatrix m3:\n" << q3 << endl;
        cout << "Trace of m3 = " << q3.trace() << endl;
                
        if (q1 ^ q2) {
            cout << "m1 ^ m2: true (trace m1 >= trace m2)" << endl;
        } else {
            cout << "m1 ^ m2: false (trace m1 < trace m2)" << endl;
        }
        
        if (q2 ^ q1) {
            cout << "m2 ^ m1: true (trace m2 >= trace m1)" << endl;
        } else {
            cout << "m2 ^ m1: false (trace m2 < trace m1)" << endl;
        }
        
        if (q3 ^ q1) {
            cout << "m3 ^ m1: true (trace m3 >= trace m1)" << endl;
        } else {
            cout << "m3 ^ m1: false (trace m3 < trace m1)" << endl;
        }
        
        if (q1 ^ q3) {
            cout << "m1 ^ m3: true (trace m1 >= trace m3)" << endl;
        } else {
            cout << "m1 ^ m3: false (trace m1 < trace m3)" << endl;
        }
        
        if (q1 ^ q1) {
            cout << "m1 ^ m1: true (trace m1 >= trace m1)" << endl;
        }


        
    } catch (const matrix::error& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "UNKNOWN ERROR" << endl;
        return 1;
    }
    
    return 0;
}
