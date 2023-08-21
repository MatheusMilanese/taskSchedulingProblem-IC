#ifndef HEURISTICA_H
#define HUERISTICA_H

#include <bits/stdc++.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>

using namespace std;

vector<int> algoritmoGenetico(vector<vector<int>> machineJobTime, vector<int> deliveryDates, int sizePopulation, int maxIterations);

#endif // HEURISTICA_H