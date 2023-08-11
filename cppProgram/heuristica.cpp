#include "heuristica.h"

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
            tempoDeConclusao[i][j] = max(tempoDeConclusao[i-1][j], tempoDeConclusao[i][j-1]);
            tempoDeConclusao[i][j] += machineJobTime[indexOrder[i]][j];
        }
    }

    if(tempoDeConclusao[qtdJobs-1][qtdMaquinas-1] < deliveryDates[indexOrder[qtdJobs-1]]){
        tempoDeConclusao[qtdJobs-1][qtdMaquinas-1] = deliveryDates[indexOrder[qtdJobs-1]];
    }

    for(int i = qtdJobs-2; i > 0; i--){
        // verifica se a tarefa i está adiantada
        if(tempoDeConclusao[i][qtdMaquinas-1] < deliveryDates[indexOrder[i]]){ 
            // verifica se a tarefa i+1 é processada depois da data de entrega da tarefa i
            if(tempoDeConclusao[i+1][qtdMaquinas-1]-machineJobTime[indexOrder[i+1]][qtdMaquinas-1] >= deliveryDates[indexOrder[i]]){ 
                tempoDeConclusao[i][qtdMaquinas-1] = deliveryDates[indexOrder[i]];
            }
            else{
                int k = i;
                int qtdTarefasAdiantadas = 1;
                int qtdTarefasJuntas = 1;
                while(k < qtdJobs-1 && tempoDeConclusao[k+1][qtdMaquinas-1]-machineJobTime[indexOrder[k+1]][qtdMaquinas-1] == tempoDeConclusao[k][qtdMaquinas-1]){
                    if(tempoDeConclusao[k+1][qtdMaquinas-1] < deliveryDates[indexOrder[k+1]]){
                        qtdTarefasAdiantadas++;
                    }
                    k++;
                    qtdTarefasJuntas++;
                }
                if(qtdTarefasAdiantadas > qtdTarefasJuntas/2){
                    for(int j = i; j <= k; j++){
                        tempoDeConclusao[j][qtdMaquinas-1]++;
                    }
                    i++;
                }
            }
        }
        
    }

    return tempoDeConclusao;
}

int funcaoObjetivo(vector<vector<int>> machineJobTime, vector<int> deliveryDates, vector<int> indexOrder){
    int valorObjetivo = 0;
    vector<vector<int>> tempoDeConclusao = GerarTempoDeConclusao(machineJobTime, deliveryDates, indexOrder);

    for(int i = 1; i < deliveryDates.size(); i++){
        valorObjetivo += std::abs(tempoDeConclusao[i][tempoDeConclusao[1].size()-1] - deliveryDates[indexOrder[i]]);
    }
    
    return valorObjetivo;
}

/*#######################################################
             B U S C A      L O C A L
  #######################################################*/

vector<int> melhoraPorTroca(vector<vector<int>> machineJobTime, vector<int> deliveryDates, vector<int> indexOrder){
    int valorObjetivo = funcaoObjetivo(machineJobTime, deliveryDates, indexOrder);
    vector<int> melhorOrdem = indexOrder;
    vector<int> ordemBase = indexOrder;
    cout << "obj: " << valorObjetivo << " | ";
    for(auto x : indexOrder) cout << x << " ";
    cout  << endl;
    for(int i = 1; i < indexOrder.size()-1; i++){
        for(int j = i+1; j < indexOrder.size(); j++){
            indexOrder[i] = ordemBase[j];
            indexOrder[j] = ordemBase[i];
            int novoValorObjetivo = funcaoObjetivo(machineJobTime, deliveryDates, indexOrder);
            cout << "obj: " << novoValorObjetivo << " | ";
            for(auto x : indexOrder) cout << x << " ";
            cout  << endl;
            if(novoValorObjetivo < valorObjetivo){
                valorObjetivo = novoValorObjetivo;
                melhorOrdem = indexOrder;
            }
            indexOrder[i] = ordemBase[i];
            indexOrder[j] = ordemBase[j];
        }
        
        indexOrder = melhorOrdem;
        ordemBase = indexOrder;
    }

    cout << "Valor da função objetivo: " << valorObjetivo << endl;


    return melhorOrdem;
}

int tempoTotal(vector<int> jobTime){
    int tempo = 0;
    for(auto x : jobTime){
        tempo += x;
    }
    return tempo;
}

vector<int> heuristicaOrdemCrescente(vector<vector<int>> machineJobTime, vector<int> deliveryDates){
    vector<int> indexOrder(deliveryDates.size());
    indexOrder[1] = 1;
    for(int i = 2; i < machineJobTime.size(); i++){
        int k = i-1;
        while(k >= 1 && 
              deliveryDates[i] < deliveryDates[indexOrder[k]] ||
              (deliveryDates[i] == deliveryDates[indexOrder[k]] && tempoTotal(machineJobTime[i]) > tempoTotal(machineJobTime[indexOrder[k]]))){
            indexOrder[k+1] = indexOrder[k];
            k--;
        }
        indexOrder[k+1] = i;
    }

    cout << "######## HEURISTICA #########\n";

    indexOrder = melhoraPorTroca(machineJobTime, deliveryDates, indexOrder);

    cout << "Ordem das tarefas: ";
    for(int i = 1; i < indexOrder.size(); i++)
        cout << indexOrder[i] << " \n"[i == indexOrder.size()-1];
    cout << "############################\n";

    return indexOrder;
}

/*#######################################################
           A L G O R I T M O     G E N É T I C O 
  #######################################################*/

int getRandomNumber(int min, int max) {
    random_device rd;
    mt19937 rng(rd()); 
    uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}

vector<int> algoritmoGenetico(vector<vector<int>> machineJobTime, vector<int> deliveryDates, int _sizePopulation, int _maxIterations){
    vector<vector<int>> population(_sizePopulation, vector<int>(deliveryDates.size()));

    for(int i = 1; i < population[0].size(); i++){
        population[0][i] = i;
        population[1][i] = i;
    }
    int valorObjetivo = funcaoObjetivo(machineJobTime, deliveryDates, population[0]);
    int iteration = 0, maxIterations = _maxIterations, indexSolution;
    bool newBestSolution;

    while(iteration++ < maxIterations || newBestSolution){
        newBestSolution = false;
        shuffle(population[1].begin()+1, population[1].end(), default_random_engine(random_device()()));
        for(int i = 2; i < _sizePopulation; i++){

            for(int j = 1; j < population[0].size(); j++){
                population[i][j] = -1;
            }
            //crossover
            int start = getRandomNumber(1, deliveryDates.size()-2);
            int end = getRandomNumber(start, deliveryDates.size()-1);
            
            // genes do pai1
            for(int j = start; j <= end; j++){
                population[i][j] = population[0][j];
            }

            //genes do pai2
            for(int j = 1; j < population[0].size(); j++){
                if(j < start || j > end){
                    int idx = j;
                    while(find(population[i].begin(), population[i].end(), population[1][idx]) != population[i].end()) {
                        idx++;
                        if(idx >= deliveryDates.size()) idx = 1;
                    }
                    population[i][j] = population[1][idx];
                }
            }

            //mutacao
            int gen1 = getRandomNumber(1, population[0].size()-1);
            int gen2 = getRandomNumber(1, population[0].size()-1);

            int aux = population[i][gen2];
            population[i][gen2] = population[i][gen1];
            population[i][gen1] = aux;

            // seleção do melhor individuo
            int novoValorObjetivo = funcaoObjetivo(machineJobTime, deliveryDates, population[i]);

            if(novoValorObjetivo < valorObjetivo){
                newBestSolution = true;
                valorObjetivo = novoValorObjetivo;
                indexSolution = i;
            }
        }
        if(newBestSolution){
            for(int j = 1; j < population[0].size(); j++){
                population[0][j] = population[indexSolution][j];
            }
        }
    }

    cout << "############ ALGORITMO GENETICO #############\n";
    cout << "tamanho da populacao: " << _sizePopulation << endl;
    cout << "numero de gerações: " << maxIterations << endl;
    cout << "valor da função objetivo: " << valorObjetivo << endl;
    cout << "Ordem das tarefas: ";
    for(int i = 1; i < population[0].size(); i++)
        cout << population[0][i] << " \n"[i == population[0].size()-1];
    cout << "############################\n";

    return population[0];
}