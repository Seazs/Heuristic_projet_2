#ifndef PFSP_H
#define PFSP_H

#include <vector>
#include <string>
#include <functional>



struct Job {
    int id;
    std::vector<int> processingTimes;
};

struct Machine {
    std::vector<int> jobOrder;
    int currentTime;
};

class PFSP {
public:
    PFSP(const char filename[]);
    ~PFSP();
    int getMakespan(const std::vector<int> jobsOrder);
    void computeMakespanTable(const std::vector<int>& jobsOrder, int** makespanTable);
    void updateMakespanTable(int** makespanTable, const std::vector<int>& jobsOrder, int startIndex);
    void copyMakespanTable(int** dest, int** src, int rows, int cols);
    int getTotalCompletionTime(const std::vector<int>& jobsOrder, int** makespanTable);
    int getTotalCompletionTime(const std::vector<int>& jobsOrder);


    // initial solution generation
    std::vector<int> generateRandomSolution();
    std::vector<int> simplifiedRZHeuristic();

    // improvement methods
    void transpose(std::vector<int>& jobsOrder, int i, int j);
    void exchange(std::vector<int>& jobsOrder, int i, int j);
    void insert(std::vector<int>& jobsOrder, int i, int j);

    // iterative methods
    std::vector<int> iterative_improvement_first(std::vector<int> jobsOrder, std::function<void(std::vector<int>&, int, int)> neighboor_function, std::string neighborhoodType, std::vector<std::pair<int, int>>& indices);
    std::vector<int> iterative_improvement_best(std::vector<int> jobsOrder, std::function<void(std::vector<int>&, int, int)> neighboor_function, std::string neighborhoodType);

    // main algo method
    std::vector<int> iterative_improvement_algorithm(std::string& improvementType, std::string& neighborhoodType, std::string& initType);

    // VND method
    std::vector<int> variable_neighborhood_descent_first(std::vector<int> neighborhoodOrder);

    void importData(const char filename[]);


    int instanceNumber;
    std::vector<Job> jobs;
    int** makespanTable;
    unsigned int numMachines;
    unsigned int numJobs;
    int processingTimes;
    


private:
    std::string filename;
    int makespan;
    int totalFlowtime;
};


#endif