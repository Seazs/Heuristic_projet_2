#ifndef CONSTANT_H
#define CONSTANT_H
#include <unordered_map>



const std::unordered_map<int, int> timeLimits = {
    {50, 1},
    {100, 1},
    {200, 1}
};

// ILS algorithm parameters
#define GAMMA 4 // as it is in the reference paper
#define LAMBDA 2 // as it is in the reference paper

//genetic algorithm parameters
#define POPULATION_SIZE 50
#define MUTATION_RATE 0.1
#define CROSSOVER_RATE 0.8
#define SEED 42

#endif // CONSTANT_H