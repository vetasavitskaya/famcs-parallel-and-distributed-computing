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
s = []
l1 = []
l2 = []

fig, fist_matrix_plot = plt.subplots()
for j in range(5):
    s.append(float(sequential[j.__str__()]))
    l1.append(float(first_loop[j.__str__()]))
    l2.append(float(second_loop[j.__str__()]))
fist_matrix_plot.plot(blocks_sizes, s, label='Sequential for matrix : ' + matrices_sizes[0].__str__())
fist_matrix_plot.plot(blocks_sizes, l1, label='First loop for matrix : ' + matrices_sizes[0].__str__())
fist_matrix_plot.plot(blocks_sizes, l2, label='Second loop for matrix : ' + matrices_sizes[0].__str__())
legend = fist_matrix_plot.legend(loc='upper center', shadow=True, fontsize='x-large')
plt.show()