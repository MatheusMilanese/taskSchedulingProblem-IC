#include "heuristica.h"

using namespace std;

/*#######################################################
             F U N Ç Õ E S     I N T E R N A S
  #######################################################*/

vector<vector<int>> GerarTempoDeConclusao(vector<vector<int>> machineJobTime, vector<int> deliveryDates, vector<int> indexOrder){
    int qtdJobs = machineJobTime.size();
    int qtdMaquinas = machineJobTime[1].size();
    vector<vector<int>> tempoDeConclusao(qtdJobs, vector<int>(qtdMaquinas));

    tempoDeConclusao[1][1] = machineJobTime[indexOrder[1]][1];
    for(int i = 1; i < qtdMaquinas; i++){
        tempoDeConclusao[1][i] = tempoDeConclusao[1][i-1] + machineJobTime[indexOrder[1]][i];
    }
    for(int i = 2; i < qtdJobs; i++){
        tempoDeConclusao[i][1] = tempoDeConclusao[i-1][1] + machineJobTime[indexOrder[i]][1];
        for(int j = 2; j < qtdMaquinas; j++){
            tempoDeConclusao[i][j] = max(tempoDeConclusao[i-1][j], tempoDeConclusao[i][j-1]) + machineJobTime[indexOrder[i]][j];
        }
    }

    if(tempoDeConclusao[qtdJobs-1][qtdMaquinas-1] < deliveryDates[indexOrder[qtdJobs-1]]){
        tempoDeConclusao[qtdJobs-1][qtdMaquinas-1] = deliveryDates[indexOrder[qtdJobs-1]];
    }

    for(int i = qtdJobs-2; i > 0; i--){
        // verifica se a tarefa i está adiantada
        if(tempoDeConclusao[i][qtdMaquinas-1] < deliveryDates[indexOrder[i]]){ 
            // verifica se a tarefa i+1 é processada depois da data de entrega da tarefa i
            if(tempoDeConclusao[i+1][qtdMaquinas-1]-machineJobTime[i+1][qtdMaquinas-1] >= deliveryDates[indexOrder[i]]){ 
                tempoDeConclusao[i][qtdMaquinas-1] = deliveryDates[indexOrder[i]];
            }
            else{
                int k = i;
                int qtdTarefasAdiantadas = 1;
                int qtdTarefasJuntas = 1;
                cout << k << " " << tempoDeConclusao[k+1][qtdMaquinas-1] << " " << machineJobTime[indexOrder[k+1]][qtdMaquinas-1] << " " << tempoDeConclusao[k][qtdMaquinas-1] << endl;
                while(k < qtdJobs-1 && tempoDeConclusao[k+1][qtdMaquinas-1]-machineJobTime[indexOrder[k+1]][qtdMaquinas-1] == tempoDeConclusao[k][qtdMaquinas-1]){
                    cout << k << " ";
                    if(tempoDeConclusao[k+1][qtdMaquinas-1] < deliveryDates[indexOrder[k+1]]){
                        qtdTarefasAdiantadas++;
                    }
                    k++;
                    qtdTarefasJuntas++;
                }
                cout << endl;
                if(qtdTarefasAdiantadas > qtdTarefasJuntas/2){
                    for(int j = i; j <= k; j++){
                        tempoDeConclusao[j][qtdMaquinas-1]++;
                    }
                    i++;
                }
            }
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

