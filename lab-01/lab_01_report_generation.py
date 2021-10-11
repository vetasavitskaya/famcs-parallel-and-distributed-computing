import pandas as pd
import json

results = {}
file = open("/home/veta/CLionProjects/famcs-parallel-and-distributed-computing/cmake-build-debug/results.csv", "r")
for line in file:
    data = pd.read_json(line)
    with pd.option_context('display.max_rows', None, 'display.max_columns', None):
        print(data)
