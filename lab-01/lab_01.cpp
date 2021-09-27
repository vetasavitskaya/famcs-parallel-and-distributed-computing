//
// Created by veta on 22.09.21.
//

#include <iostream>
#include <random>
#include <cstdlib>
#include <omp.h>


using namespace std;

void print_matrix(const vector<vector<int>> &matrix) {
    for (auto & row : matrix) {
        for (int column : row) {
            cout.width(8);
            cout << column;
        }
        cout << endl;
    }
}

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

void linear_variant_of_multiplication(vector<vector<int>>& matrix_A_, vector<vector<int>>& matrix_B_, vector<vector<int>>& matrix_C_, int parallel_num) {
    ulong n1 = matrix_A_.size();
    ulong n2 = matrix_A_[0].size();
    ulong n3 = matrix_B_[0].size();
    int i, j, k;
    omp_set_num_threads(parallel_num);
    clock_t begin_time = clock();
#pragma omp parallel for if (parallel_num == 1) default(none) shared(matrix_A_, matrix_B_, matrix_C_, n1, n2, n3, parallel_num) private(i, j, k)
    for (i = 0; i < n1; i++) {
#pragma omp parallel for if (parallel_num == 2) default(none) shared(matrix_A_, matrix_B_, matrix_C_, n1, n2, n3, i) private(j, k)
        for (j = 0; j < n3; j++) {
            for (k = 0; k < n2; k++) {
                matrix_C_[i][j] += matrix_A_[i][k] * matrix_B_[k][j];
            }
        }
    }
    cout << "Parallel time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << "\n";
}
void block_variant_of_multiplication(vector<vector<int>>& matrix_A_, vector<vector<int>>& matrix_B_, vector<vector<int>>& matrix_C_, int parallel_num, int block_size) {
    ulong n1 = matrix_A_.size();
    ulong n2 = matrix_A_[0].size();
    ulong n3 = matrix_B_[0].size();
    ulong q1 = n1 / block_size;
    ulong q2 = n2 / block_size;
    ulong q3 = n3 / block_size;
    clock_t begin_time = clock();
    int i, j, k, i1, j1, k1, i2, j2, k2;
#pragma omp parallel for if (parallel_num == 1) default(none) shared(matrix_A_, matrix_B_, matrix_C_, n1, n2, n3, q1, q2, q3, parallel_num, block_size) private(i, j, k, i1, i2, j1, j2, k1, k2)
    for (i1 = 0; i1 < q1; i1++) {
#pragma omp parallel for if (parallel_num == 2) default(none) shared(matrix_A_, matrix_B_, matrix_C_, n1, n2, n3, q1, q2, q3, parallel_num, block_size, i1) private(i, j, k, i2, j1, j2, k1, k2)
        for (j1 = 0; j1 < q2; j1++) {
            for (k1 = 0; k1 < q3; k1++) {
                for (i2 = 0; i2 < block_size; i2++) {
                    for (j2 = 0; j2 < block_size; j2++) {
                        for (k2 = 0; k2 < block_size; k2++) {
                            i = i1 * block_size + i2;
                            j = j1 * block_size + j2;
                            k = k1 * block_size + k2;
                            matrix_C_[i][j] += matrix_A_[i][k] * matrix_B_[k][j];
                        }
                    }
                }
            }
        }
    }
    cout << "Parallel time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << "\n";
}

int main(){
    int number_of_rows_A = 1500, number_of_columns_A = 1500, number_of_rows_B = 1500, number_of_columns_B = 1500;
    vector<vector<int>> matrix_A, matrix_B, matrix_C(number_of_rows_A, vector<int>(number_of_rows_B, 0));
    matrix_A = generate_matrix(number_of_rows_A, number_of_columns_A);
    matrix_B = generate_matrix(number_of_rows_B, number_of_columns_B);
    //print_matrix(matrix_A);
    //print_matrix(matrix_B);
    //print_matrix(matrix_C);
    //linear_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 2);
    //print_matrix(matrix_C);
    block_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 2, 100);
    //print_matrix(matrix_C);
    return 0;
}