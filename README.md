## Heuristic_projet_2
The initial solution is built using the Simplified RZ Heuristic, which allows the algorithm to obtain good solutions from the very first iterations.


## Key directories and files
- **`src/`**: Contains the source code for the heuristic algorithms and utility functions.
  - `genetic.cpp` and `genetic.hpp`: Implementation of the Genetic Algorithm.
  - `ils.cpp` and `ils.hpp`: Implementation of the Iterated Local Search algorithm.
  - `PFSP.cpp` and `PFSP.hpp`: Core classes and functions for the Permutation Flow Shop Problem (PFSP).
  - `constant.hpp`: Defines constants used across the project.
  - `utils.cpp` and `utils.hpp`: Utility functions

- **`data/`**: Includes benchmark instances and best-known solutions for PFSP.
  - `Benchmarks/`: Contains problem instances for testing.
  - `bestKnownTCT.txt`: Stores the best-known Total Completion Times (TCT) for comparison.

- **`results/`**: Stores the output CSV files containing results of algorithm runs.
  - Files are named with timestamps, e.g., `results_YYYYMMDD_HHMMSS.csv`.

- **`Scripts/`**: Python scripts for running benchmarks, analyzing and visualizing results.
  - `run_experiments.py` : Runs the algorithms on the benchmark instances and saves the results in CSV format.
  - `analyze result.py`: Analyzes the performance of algorithms and generates plots.

- **`PDF/`**: Contains reference papers and documentation.
  - `1-s2.0-S0377221712003426-main.pdf`: A reference paper on PFSP heuristics.
  - `Implementation_2_2025.pdf`: Documentation of the implementation.
  - `Report2.pdf`: Report on the project.

- **`build/`**: Compiled object files generated during the build process.

- **`Makefile`**: Automates the build process for the project.



## Building the Project
To build the project, navigate to the root directory and run:

```
make
```

This will compile all the `.cpp` files in the `src` folder and create an executable named `Heuristic_PFSP_SLS` in the root directory.

## Dependencies
- **C++ Compiler**: Ensure you have a C++ compiler installed (e.g., g++, clang++).
- **Python**: Required for running the `run_experiments.py` and `analyze_result.py` scripts.
- **Matplotlib**: Required for plotting results
- **Pandas**: Required for data manipulation and analysis. 


## Running the Project

After building the project, you can run the executable with the following command:

´´´
./Heuristic_PFSP_SLS <instance_file> <algorithm>
´´´

Where `<instance_file>` is the path to the benchmark instance file (e.g., `data/Benchmarks/instance1.txt`) and `<algorithm>` specifies the algorithm to use (e.g., `--genetic` or `--ils`).

for example:
```
./Heuristic_PFSP_SLS data/Benchmarks/ta051 --genetic
```
or

```
./Heuristic_PFSP_SLS data/Benchmarks/ta101 --ils
```


## Benchmarking
Running the `run_experiments.py` script will execute the algorithms on all benchmark instances and save the results in a CSV file. 

Then you can analyze the results using the `analyze_result.py` script, which generates plots and statistics based on the output CSV files.
