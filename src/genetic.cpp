#include "genetic.hpp"
#include "PFSP.hpp"
#include "utils.hpp"
#include "constant.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cmath>
#include <unordered_map>
#include <unordered_set>



std::vector<int> genetic(PFSP& pfsp) {
    static std::mt19937 gen(SEED);
    std::uniform_real_distribution<double> uniform(0, 1);

    // Initialization
    std::vector<std::vector<int>> population;
    population.push_back(pfsp.simplifiedRZHeuristic()); // Add an initial solution using RZ heuristic

    for (int i = 0; i < POPULATION_SIZE - 1; ++i) {
        population.push_back(pfsp.generateRandomSolution()); // Add random solutions to the population
    }

    std::vector<int> bestSolution = population[0];
    int bestScore = pfsp.getTotalCompletionTime(bestSolution);
    size_t unsuccessful = 0;

    // define the neighboor function
    std::function<void(std::vector<int>&, int, int)> neighboor_function;
    neighboor_function = [&pfsp](std::vector<int>& jobsOrder, int i, int j) {
        return pfsp.insert(jobsOrder, i, j);
    };

    //generate all the pairs of indices in random order
    std::vector<std::pair<int, int>> indices;
    for (int i = 0; i < pfsp.numJobs; ++i) {
        for (int j = 0; j < pfsp.numJobs; ++j) {
            if (i != j) {
                indices.emplace_back(i, j);
            }
        }
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(indices.begin(), indices.end(), gen);

    // Start timing
    const clock_t begin_time = clock();
    int timeLimit = timeLimits.at(pfsp.numJobs);


    //std::cout << "starting genetic algorithm..." << std::endl;

    while ((float(clock() - begin_time) / CLOCKS_PER_SEC) <= timeLimit) {
        // Generate a new population using crossover
        //std::cout << "Generating new population..." << std::endl;
        std::vector<std::vector<int>> nextPopulation;
        for (size_t i = 0; i < population.size(); ++i) {
            std::vector<int> parent1 = population[i];
            std::vector<int> parent2 = population[(i + 1) % population.size()];

            if (uniform(gen) < CROSSOVER_RATE) {
                nextPopulation.push_back(crossover(parent1, parent2, gen));
            } else {
                nextPopulation.push_back(parent1);
            }
        }

        //std::cout << "Mutating new population..." << std::endl;
        // Mutate solutions
        for (auto& individual : nextPopulation) {
            if (uniform(gen) < (MUTATION_RATE * std::sqrt(unsuccessful + 1))) {
                mutate(individual, pfsp, gen);
            }
        }
        // Apply iterative improvement (local search) on each solution
        for (auto& individual : nextPopulation) {
            //std::cout << "Applying iterative improvement..." << std::endl;
            individual = pfsp.iterative_improvement_first(individual,neighboor_function, "insert", indices);
            // std::cout<< "Finished applying iterative improvement..." << std::endl;
            // std::cout<< ((float(clock() - begin_time) / CLOCKS_PER_SEC)) << std::endl;
        }
        

        // Merge the old and new populations
        population.insert(population.end(), nextPopulation.begin(), nextPopulation.end());

        // Select the best subset of the population
        std::sort(population.begin(), population.end(), [&pfsp](const std::vector<int>& a, const std::vector<int>& b) {
            return pfsp.getTotalCompletionTime(a) < pfsp.getTotalCompletionTime(b);
        });
        population.resize(POPULATION_SIZE); // Keep only the top solutions

        // Find the best solution in the current population
        std::vector<int> bestOfPopulation = population[0];
        int score = pfsp.getTotalCompletionTime(bestOfPopulation);

        // Update the best solution if a better one is found
        if (score < bestScore) {
            bestScore = score;
            bestSolution = bestOfPopulation;
            std::cout << ((float(clock() - begin_time) / CLOCKS_PER_SEC)) << " " << bestScore << std::endl;
            unsuccessful = 0;
        } else {
            ++unsuccessful;
        }
    }

    return bestSolution;
}

std::vector<int> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2, std::mt19937& rng) {
    std::uniform_int_distribution<> dis_c(0, parent1.size() - 1);
    auto crossover_point = dis_c(rng);

    std::vector<int> common(parent1.size(), -1);

    // Fill the identical jobs
    for (size_t i = 0; i < parent1.size(); ++i) {
        if (parent1[i] == parent2[i]) {
            common[i] = parent1[i];
        }
    }

    std::vector<int> offspring = common;

    // Fill up to the crossover point
    for (int i = 0; i < crossover_point; ++i) {
        offspring[i] = parent1[i];
    }

    // Fill the rest by order from the other parent
    auto copyByOrder = [](const std::vector<int>& source, std::vector<int>& target) {
        std::unordered_set<int> existing(target.begin(), target.end());
        size_t targetIndex = 0;
        for (int job : source) {
            while (targetIndex < target.size() && target[targetIndex] != -1) {
                ++targetIndex;
            }
            if (existing.find(job) == existing.end() && targetIndex < target.size()) {
                target[targetIndex] = job;
                existing.insert(job);
            }
        }
    };

    copyByOrder(parent2, offspring);

    return offspring;
}

void mutate(std::vector<int>& individual, PFSP& pfsp, std::mt19937& rng) {
    // Swap mutation
    std::uniform_int_distribution<int> dist(0, individual.size() - 1);
    int index1 = dist(rng);
    int index2 = dist(rng);
    std::swap(individual[index1], individual[index2]);
}