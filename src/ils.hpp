#ifndef ILS_H
#define ILS_H

#include <iostream>
#include <vector>
#include <string>


#include "PFSP.hpp"
#include "utils.hpp"


std::vector<int> ils(PFSP& pfsp);
void perturb_solution(std::vector<int>& solution, PFSP& pfsp);
bool accept_criterion(PFSP& pfsp, int currentTCT, int perturbedTCT);


#endif // ILS_H