//
// Created by veta on 22.09.21.
//

#include <iostream>
#include <random>
#include <cstdlib>

using namespace std;

vector<vector<int>> generate_matrix(int rows, int columns);
void print_matrix(const vector<vector<int>>& matrix);

vector<vector<int>> generate_matrix(int rows, int columns) {
    vector<vector<int>> matrix;
    for (int row = 0; row < rows; row++) {
        vector<int> matrix_row;
        matrix_row.reserve(columns);
        for (int column = 0; column < columns; column++) {
            matrix_row.push_back((rand() % 200) - 100);
        }
        matrix.push_back(matrix_row);
    }
    return matrix;
}

void print_matrix(const vector<vector<int>> &matrix) {
    for (auto & row : matrix) {
        for (int column : row) {
            cout.width(8);
            cout << column;
        }
        cout << endl;
    };
}

void linear_multiply(vector<vector<int>>& matrix_A_, vector<vector<int>>& matrix_B_, vector<vector<int>>& matrix_C_) {
    ulong n1 = matrix_A_.size();
    ulong n2 = matrix_A_[0].size();
    ulong n3 = matrix_B_[0].size();
    clock_t begin_time = clock();
    int i, j, k;
#pragma omp parallel shared ( matrix_A_, matrix_B_, matrix_C_, n1, n2, n3) private ( i, j, k ) default(none)
#pragma omp for
    for (i = 0; i < n1; i++) {
        for (j = 0; j < n3; j++) {
            for (k = 0; k < n2; k++) {
                matrix_C_[i][j] += matrix_A_[i][k] * matrix_B_[k][j];
            }
        }
    }
    cout << "Parallel time: "<<float( clock () - begin_time ) / CLOCKS_PER_SEC  <<"\n";
}

int main(){
    int number_of_rows_A = 4 , number_of_columns_A = 4 , number_of_rows_B = 4, number_of_columns_B = 4;
    vector<vector<int>> matrix_A, matrix_B, matrix_C(number_of_rows_A, vector<int>(number_of_rows_B, 0));
    matrix_A = generate_matrix(number_of_rows_A, number_of_columns_A);
    matrix_B = generate_matrix(number_of_rows_B, number_of_columns_B);
    print_matrix(matrix_A);
    print_matrix(matrix_B);
    print_matrix(matrix_C);
    linear_multiply(matrix_A, matrix_B, matrix_C);
    print_matrix(matrix_C);
    return 0;
}