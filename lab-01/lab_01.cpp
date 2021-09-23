//
// Created by veta on 22.09.21.
//
#include <iostream>
#include <random>
#include <cstdlib>

using namespace std;

vector<vector<int>> generate_matrix(int rows, int columns){
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

void print_matrix(const vector<vector<int>>& matrix){
    for (auto & row : matrix) {
        for (int column : row) {
            cout.width(6);
            cout << column;
        }
        cout << endl;
    };
}

void linear_multiply(vector<vector<int>>& matrix_A_, vector<vector<int>>& matrix_B_, vector<vector<int>>& matrix_C_) {
    ulong n_1 = matrix_A_.size();
    ulong n_2 = matrix_A_[0].size();
    ulong n_3 = matrix_B_[0].size();
#pragma omp parallel for if (parallel_num == 1)
    for (int i = 0; i < n_1; i++) {
#pragma omp parallel for if (parallel_num == 2)
        for (int j = 0; j < n_3; j++) {
            for (int k = 0; k < n_2; k++) {
                matrix_C_[i][j] += matrix_A_[i][k] * matrix_B_[k][j];
            }
        }
    }
}

void block_multiply(vector<vector<int>> matrix_1, vector<vector<int>> matrix_2){

}
int main(){
    int number_of_rows_A = 10 , number_of_columns_A = 10 , number_of_rows_B = 10, number_of_columns_B = 10;
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