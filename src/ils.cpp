#include "ils.hpp"
#include "PFSP.hpp"
#include "utils.hpp"
#include "constant.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <unordered_map>
#include <random>
#include <chrono>
#include <algorithm>


std::vector<int> ils(PFSP& pfsp) {
    //std::cout << "Starting ILS..." << std::endl;
    // Generate an initial solution using the simplified RZ heuristic
    std::vector<int> bestSolution = pfsp.simplifiedRZHeuristic();

    //define the neighboor function
    std::function<void(std::vector<int>&, int, int)> neighboor_function;
    neighboor_function = [&pfsp](std::vector<int>& jobsOrder, int i, int j) {
                return pfsp.insert(jobsOrder, i, j);
            };
    //std::cout << "computing indices..." << std::endl;
    // generate all pairs of indices in random order.
    // this order will be the same for all iterations
    std::vector<std::pair<int, int>> indices;
    for (int i = 0; i < pfsp.numJobs; ++i) {
        for (int j = 0; j < pfsp.numJobs; ++j) {
            if (i != j) {
                indices.emplace_back(i, j);
            }
        }
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 g(seed);           
    std::shuffle(indices.begin(), indices.end(), g);

    //std::cout << "applying first iterative improvement..." << std::endl;
    // aply the iterative improvement method
    std::vector<int> newSolution = pfsp.iterative_improvement_first(bestSolution, neighboor_function, "insert", indices);
    int bestTCT = pfsp.getTotalCompletionTime(bestSolution);
    std::vector<int> currentSolution = bestSolution;
    int currentTCT = bestTCT;

    //while loop that have a time limit depending on the instance size

    const clock_t startTime = clock();
    int timeLimit = timeLimits.at(pfsp.numJobs);
    //std::cout << "starting perturbation loop..." << std::endl;
    while(( clock() - startTime ) / CLOCKS_PER_SEC < timeLimit) {

        //std::cout<< "Perturbing solution..." << std::endl;
        perturb_solution(currentSolution, pfsp);
        // Apply the iterative improvement method to the perturbed solution
        //std::cout<< "Applying iterative improvement..." << std::endl;
        newSolution = pfsp.iterative_improvement_first(currentSolution, neighboor_function, "insert", indices);
        // Calculate the total completion time of the perturbed solution
        int perturbedTCT = pfsp.getTotalCompletionTime(newSolution, pfsp.makespanTable);
        // If the perturbed solution is better than the current best solution, update it
        //std::cout<< "currentTCT: " << currentTCT << " | perturbedTCT: " << perturbedTCT << std::endl;
        //std::cout<< "bestTCT: " << bestTCT << std::endl;
        if (perturbedTCT < currentTCT) {
            currentSolution = newSolution;
            currentTCT = perturbedTCT;
            if (currentTCT < bestTCT) {
                bestSolution = currentSolution;
                bestTCT = currentTCT;
                std::cout << ((float( clock () - startTime ) /  CLOCKS_PER_SEC)) << " " <<  bestTCT << std::endl;
            }
            
        }else{
            if (accept_criterion(pfsp, currentTCT, perturbedTCT)) {
                currentSolution = newSolution;
                currentTCT = perturbedTCT;
                std::cout << ((float( clock () - startTime ) /  CLOCKS_PER_SEC)) << " " <<  bestTCT << std::endl;
            }
        }
    }
    return bestSolution;
}

void perturb_solution(std::vector<int>& solution, PFSP& pfsp) {
    // randomly apply gamma insert moves
    static std::mt19937 rng(SEED);
    std::uniform_int_distribution<int> dist(0, solution.size() - 1);
    for (int i = 0; i < GAMMA; ++i) {
        int index1 = dist(rng);
        int index2 = dist(rng);
        while (index1 == index2) {
            index2 = dist(rng);
        }
        // Apply the insert move
        pfsp.insert(solution, index1, index2);
    }  
}

bool accept_criterion(PFSP& pfsp, int currentTCT, int perturbedTCT) {
    // Calculate the difference in total completion time
    int delta = perturbedTCT - currentTCT;

    // If the perturbed solution is better, accept it
    if (delta < 0) {
        std::cout << "Solution should have been accepted before" << std::endl;
        return true;
    }

    int processingTime = pfsp.processingTimes; // constant for all jobs so it is computed only once

    auto temperature = (LAMBDA * processingTime) / (10 * pfsp.numJobs * pfsp.numMachines);

    // Generate a random number between 0 and 1
    static std::mt19937 rng(SEED);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    
    return uniform(rng) < exp(-delta / temperature);
}