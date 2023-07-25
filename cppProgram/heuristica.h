#ifndef HEURISTICA_H
#define HUERISTICA_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

vector<int> heuristicaOrdemCrescente(vector<vector<int>> machineJobTime, vector<int> deliveryDates);
vector<int> algoritmoGenetico(vector<vector<int>> machineJobTime, vector<int> deliveryDates, int sizePopulation, int maxIterations);

#endif // HEURISTICA_H