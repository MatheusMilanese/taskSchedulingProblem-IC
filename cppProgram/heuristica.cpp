#include "heuristica.h"

using namespace std;

/*#######################################################
             F U N Ç Õ E S     I N T E R N A S
  #######################################################*/


vector<vector<int>> GerarTempoDeConclusao(vector<vector<int>> machineJobTime, vector<int> deliveryDates, vector<int> indexOrder){
    vector<vector<int>> tempoDeConclusao(machineJobTime.size(), vector<int>(machineJobTime[1].size()));


    tempoDeConclusao[1][1] = machineJobTime[indexOrder[1]][1];
    for(int i = 1; i < machineJobTime[1].size(); i++){
        tempoDeConclusao[1][i] = tempoDeConclusao[1][i-1] + machineJobTime[indexOrder[1]][i];
    }
    for(int i = 2; i < machineJobTime.size(); i++){
        tempoDeConclusao[i][1] = tempoDeConclusao[i-1][1] + machineJobTime[indexOrder[i]][1];
        for(int j = 2; j < machineJobTime[1].size(); j++){
            tempoDeConclusao[i][j] = max(tempoDeConclusao[i-1][j], tempoDeConclusao[i][j-1]) + machineJobTime[indexOrder[i]][j];
        }
    }
    cout << "######## tempo de conclusao #########\n";
    for(int i = 1; i < tempoDeConclusao.size(); i++)
        for(int j = 1; j < tempoDeConclusao[1].size(); j++)
            cout << tempoDeConclusao[i][j] << " \n"[j == tempoDeConclusao[1].size()-1];
    cout << "############################\n";

    return tempoDeConclusao;
}



void funcaoObjetivo(vector<vector<int>> machineJobTime, vector<int> deliveryDates, vector<int> indexOrder){
    int valorObjetivo = 0;
    vector<vector<int>> tempoDeConclusão = GerarTempoDeConclusao(machineJobTime, deliveryDates, indexOrder);

    for(int i = 1; i < deliveryDates.size(); i++){
        valorObjetivo += abs(tempoDeConclusão[i][tempoDeConclusão[1].size()-1] - deliveryDates[indexOrder[i]]);
    }
    
    cout << "#####################\n";
    cout << "Valor objetivo " << valorObjetivo << endl;
    cout << "#####################\n";
}

vector<int> heuristicaOrdemCrescente(vector<vector<int>> machineJobTime, vector<int> deliveryDates){
    vector<int> indexOrder(deliveryDates.size());
    indexOrder[1] = 1;
    for(int i = 2; i < machineJobTime.size(); i++){
        int k = i-1;
        while(k >= 1 && deliveryDates[i] < deliveryDates[indexOrder[k]]){
            indexOrder[k+1] = indexOrder[k];
            k--;
        }
        indexOrder[k+1] = i;
    }

    cout << "######## HEURISTICA #########\n";
    for(int i = 1; i < indexOrder.size(); i++)
        cout << indexOrder[i] << " \n"[i == indexOrder.size()-1];
    cout << "############################\n";

    funcaoObjetivo(machineJobTime, deliveryDates, indexOrder);

    return indexOrder;
}

