//
// Created by veta on 22.09.21.
//

#include <iostream>
#include <random>
#include <cstdlib>
#include <fstream>
#include <omp.h>

using namespace std;

int sizes_of_matrices[4] = {200, 400, 800, 1600};
int sizes_of_blocks[5] = {1, 20, 50, 100, 200};


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
    }
}


float linear_variant_of_multiplication(vector<vector<int>>& matrix_A_, vector<vector<int>>& matrix_B_,
                                       vector<vector<int>>& matrix_C_, int loop) {
    unsigned long n1 = matrix_A_.size();
    unsigned long n2 = matrix_A_[0].size();
    unsigned long n3 = matrix_B_[0].size();
    matrix_C_.assign(n1, vector<int>(n3, 0));
    clock_t begin_time = clock();
#pragma omp parallel for if (loop == 1)
    for (int i = 0; i < n1; i++) {
#pragma omp parallel for if (loop == 2)
        for (int j = 0; j < n3; j++) {
            for (int k = 0; k < n2; k++) {
                matrix_C_[i][j] += matrix_A_[i][k] * matrix_B_[k][j];
            }
        }
    }
    clock_t end_time = clock();
    return float(end_time - begin_time) / CLOCKS_PER_SEC;
}


float block_variant_of_multiplication(vector<vector<int>>& matrix_A_, vector<vector<int>>& matrix_B_,
                                      vector<vector<int>>& matrix_C_, int loop, int block_size) {
    unsigned long n1 = matrix_A_.size();
    unsigned long n2 = matrix_A_[0].size();
    unsigned long n3 = matrix_B_[0].size();
    unsigned long q1 = n1 / block_size;
    unsigned long q2 = n2 / block_size;
    unsigned long q3 = n3 / block_size;
    matrix_C_.assign(n1, vector<int>(n3, 0));
    clock_t begin_time = clock();
#pragma omp parallel for if (loop == 1)
    for (int i1 = 0; i1 < q1; i1++) {
#pragma omp parallel for if (loop == 2)
        for (int j1 = 0; j1 < q2; j1++) {
            for (int k1 = 0; k1 < q3; k1++) {
                for (int i2 = 0; i2 < block_size; i2++) {
                    for (int j2 = 0; j2 < block_size; j2++) {
                        for (int k2 = 0; k2 < block_size; k2++) {
                            int i = i1 * block_size + i2;
                            int j = j1 * block_size + j2;
                            int k = k1 * block_size + k2;
                            matrix_C_[i][j] += matrix_A_[i][k] * matrix_B_[k][j];
                        }
                    }
                }
            }
        }
    }
    clock_t end_time = clock();
    return float(end_time - begin_time) / CLOCKS_PER_SEC;
}


string get_json_pair(const string& index, const string& value){
    return '"' + index + '"' + ": " + value + ", ";
}

int main(){
    ofstream results;
    omp_set_dynamic(0);
    omp_set_num_threads(4);
    results.open("results.csv");
    string size_of_block, size_of_matrix, sequential_result, first_loop_result, second_loop_result;
    int counter = 0;
    for (int sizes_of_matrix : sizes_of_matrices) {

        float sequential, first_loop, second_loop = 0;

        int number_of_rows_A = sizes_of_matrix,
                number_of_columns_A = sizes_of_matrix,
                number_of_rows_B = sizes_of_matrix,
                number_of_columns_B = sizes_of_matrix;

        vector<vector<int>> matrix_A, matrix_B, matrix_C(number_of_rows_A, vector<int>(number_of_rows_B, 0));

        matrix_A = generate_matrix(number_of_rows_A, number_of_columns_A);
        matrix_B = generate_matrix(number_of_rows_B, number_of_columns_B);

        for (int sizes_of_block : sizes_of_blocks) {

            size_of_block += get_json_pair(to_string(counter), to_string(sizes_of_block));
            size_of_matrix += get_json_pair(to_string(counter), to_string(sizes_of_matrix));

            if (sizes_of_block == 1) {

                sequential = linear_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 0);
                first_loop = linear_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 1);
                second_loop = linear_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 2);

            } else {

                sequential = block_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 0, sizes_of_block);
                first_loop = block_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 1, sizes_of_block);
                second_loop = block_variant_of_multiplication(matrix_A, matrix_B, matrix_C, 2, sizes_of_block);
            }

            sequential_result += get_json_pair(to_string(counter), to_string(sequential));
            first_loop_result += get_json_pair(to_string(counter), to_string(first_loop));
            second_loop_result += get_json_pair(to_string(counter), to_string(second_loop));
            counter++;
        }
    }
    
    results << "{ ";
    results << "\"Size of matrices\": {" << size_of_matrix.substr(0, size_of_matrix.size()-2).c_str() << "}" << ", ";
    results << "\"Size of blocks\": {" << size_of_block.substr(0, size_of_block.size()-2).c_str() << "}" << ", ";
    results << "\"Sequential\": {" << sequential_result.substr(0, sequential_result.size()-2).c_str() << "}" << ", ";
    results << "\"First loop\": {" << first_loop_result.substr(0, first_loop_result.size()-2).c_str() << "}" << ", ";
    results << "\"Second loop\": {" << second_loop_result.substr(0, second_loop_result.size()-2).c_str() << "}" << "}";
    results.close();

    return 0;

}