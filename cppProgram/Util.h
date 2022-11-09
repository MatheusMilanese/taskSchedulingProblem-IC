#ifndef ___UTIL_H__
#define ___UTIL_H__

#include <bits/stdc++.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <ilcplex/ilocplex.h>

using namespace std;

typedef IloArray<IloNumVarArray> NumVarMatrix2D;
typedef IloArray<IloBoolVarArray> BoolVarMatrix;
typedef unsigned long long llu;


//==============================================================================
//                        VARIÁVEIS GLOBAIS    
//==============================================================================

// Variáveis de entrada
int numJobs;
int numMachines;
vector<vector<llu>> machineJobTime;
vector<llu> deliveryDates;

#endif