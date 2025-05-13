#ifndef GENETIC_H
#define GENETIC_H

#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "PFSP.hpp"


std::vector<int> genetic(PFSP& pfsp);
std::vector<int> crossover(const std::vector<int>& parent1, const std::vector<int>& parent2, std::mt19937& rng);
void mutate(std::vector<int>& individual, PFSP& pfsp, std::mt19937& rng);


#endif // GENETIC_H