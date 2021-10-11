import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import json

results = {}
file = open("/home/veta/CLionProjects/famcs-parallel-and-distributed-computing/cmake-build-debug/results.csv", "r")
line = file.readline()
data = pd.read_json(line)
results = json.loads(line)
with pd.option_context('display.max_rows', None, 'display.max_columns', None):
    print(data)

matrices_sizes = [200, 400, 800, 1600]
blocks_sizes = [1, 20, 50, 100, 200]
sequential = results["Sequential"]
first_loop = results["First loop"]
second_loop = results["Second loop"]
plots = []
for i in range(4):
    _, plot_for_appropriate_matrix_size = plt.subplots()
    plots.append(plot_for_appropriate_matrix_size)
for i in range(4):
    sequential_for_appropriate_matrix_size = []
    first_loop_for_appropriate_matrix_size = []
    second_loop_for_appropriate_matrix_size = []
    for j in range(5):
        sequential_for_appropriate_matrix_size.append(float(sequential[(i * 5 + j).__str__()]))
        first_loop_for_appropriate_matrix_size.append(float(first_loop[(i * 5 + j).__str__()]))
        second_loop_for_appropriate_matrix_size.append(float(second_loop[(i * 5 + j).__str__()]))
    plots[i].plot(blocks_sizes, sequential_for_appropriate_matrix_size, label='Sequential for matrix : '
                                                                              + matrices_sizes[i].__str__())
    plots[i].plot(blocks_sizes, first_loop_for_appropriate_matrix_size, label='First loop for matrix : '
                                                                              + matrices_sizes[i].__str__())
    plots[i].plot(blocks_sizes, second_loop_for_appropriate_matrix_size, label='Second loop for matrix : '
                                                                               + matrices_sizes[i].__str__())
    legend = plots[i].legend(loc='best', shadow=True, fontsize='x-large')
plt.show()
