#ifndef UTILS_H
#define UTILS_H

// Include necessary headers
#include <iostream>
#include <vector>
#include <string>

void print_jobs_order(std::vector<int> jobsOrder);
void print_makespan_table(int** makespanTable, int rows, int cols);
void handleProgramParameters(int argc, char* argv[], std::string& dataFileName, std::string& sls_type);

#endif // UTILS_H