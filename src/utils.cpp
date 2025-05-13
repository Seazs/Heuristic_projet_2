#include <vector>
#include <string>
#include <iostream>

void print_jobs_order(std::vector<int> jobsOrder){
    for (int i = 0; i < jobsOrder.size(); ++i) {
        printf("%d ", jobsOrder[i]);
    }
    printf("\n");
}

void print_makespan_table(int** makespanTable, int rows, int cols){
    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            printf("%d ", makespanTable[i][j]);
        }
        printf("\n");
    }
}

void handleProgramParameters(int argc, char* argv[], std::string& dataFileName, std::string& sls_type) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <data-file> --<sls_type>" << std::endl;
        std::cerr << "Example: ./Heuristic_PSFP data.txt --ils" << std::endl;
        exit(EXIT_FAILURE);
    }

    dataFileName = argv[1];
    sls_type = argv[2];

    if (sls_type != "--ils" && sls_type != "--genetic") {
        std::cerr << "Invalid SLS type. Use --ils or --genetic." << std::endl;
        exit(EXIT_FAILURE);
    }
}
