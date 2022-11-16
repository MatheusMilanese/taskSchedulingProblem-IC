#include <bits/stdc++.h>
#include <limits.h>
#include <time.h>
#include <sys/time.h>
#include <ilcplex/ilocplex.h>

using namespace std;

/*--------------------------------------------------*
 *        V A R I A V E I S   G L O B A I S         *
 *--------------------------------------------------*/
int numJobs;
int numMachines;
vector<vector<int>> machineJobTime;
vector<int> deliveryDates;

int main(){
    /*--------------------------------------------------*
     *       L E I T U R A   D A   E N T R A D A        *
     *--------------------------------------------------*/

    cout << "iniciando leitura" << endl;

    cin >> numJobs >> numMachines;

    machineJobTime.resize(numJobs+1);
    deliveryDates.resize(numJobs+1);
    for(int i = 1; i <= numJobs; ++i)
        machineJobTime[i].resize(numMachines+1);

    for(int i = 1; i <= numJobs; ++i){
        for(int j = 1; j <= numMachines; ++j){
            cin >> machineJobTime[i][j];
        }
    }

    for(int i = 1; i <= numJobs; ++i)
        cin >> deliveryDates[i];

    cout << "Leitura concluida" << endl;
    /*--------------------------------------------------*
     *        C R I A N D O    O    M O D E L O         *
     *--------------------------------------------------*/
    IloEnv env;  // criando o ambiente

    try {
        cout << "Criando modelo" << endl;
        IloModel model(env, "Modelo"); // criando o modelo
        IloCplex cplex(env); //criando o cplex

        cout << "Criando variaveis de decisao" << endl;
        // Variaveis de decisao
        IloArray<IloIntVarArray> X(env, numJobs+1);   // Ordem de realização dos "jobs"
        IloIntVarArray C(env, numJobs+1, 0, IloIntMax);   // Tempo em que o "job" foi terminado
        IloIntVarArray E(env, numJobs+1, 0, IloIntMax);   // Adiantamento da entrega do "job"
        IloIntVarArray T(env, numJobs+1, 0, IloIntMax);   // Atraso na entrega do "Job"

        IloArray<IloIntVarArray> W(env, numJobs+1);
        IloArray<IloIntVarArray> I(env, numJobs);
        IloIntVar I0(env, 0, IloIntMax);

    
        for(int i = 1; i < numJobs; ++i) {
            X[i] = IloIntVarArray(env, numJobs+1, 0, IloIntMax);
            W[i] = IloIntVarArray(env, numMachines, 0, IloIntMax);
            I[i] = IloIntVarArray(env, numMachines+1, 0, IloIntMax);
        }
        X[numJobs] = IloIntVarArray(env, numJobs+1, 0, IloIntMax);
        W[numJobs] = IloIntVarArray(env, numMachines, 0, IloIntMax);

        // Funçao Objetivo
        IloExpr Objetivo(env);
        cout << "Criando funcao objetivo" << endl;
        for(int i = 1; i <= numJobs; i++){
            Objetivo += (E[i] + T[i]);
        }

        cout << "Adicionando objetivo ao modelo" << endl;
        model.add(IloMinimize(env, Objetivo));

        cout << "Criando restricoes" << endl;
        // Restricoes
        IloConstraintArray restricoes(env);

        IloExpr tempoTarefaInicial(env);
        for(int k = 1; k <= numMachines; ++k){
            for(int i = 1; i <= numJobs; ++i)
                tempoTarefaInicial += X[i][1] * machineJobTime[i][k];
        }
        for(int i = 1; i < numMachines; ++i) 
            tempoTarefaInicial += W[1][i]; 
        
        restricoes.add(C[1] == I0 + tempoTarefaInicial);

        cout << "bloco de restricao 1 adicionado" << endl;

        IloExprArray tempoTarefas(env, numJobs+1);
        IloExprArray somatorioLinhaX(env, numJobs+1); 
        IloExprArray somatorioColunaX(env, numJobs+1);
        for(int i = 1; i <= numJobs; ++i){
            tempoTarefas[i] = IloExpr(env);
            somatorioLinhaX[i] = IloExpr(env);
            somatorioColunaX[i] = IloExpr(env);
        }
        
        for(int j = 2; j <= numJobs; ++j){
            for(int i = 1; i <= numJobs; ++i){
                tempoTarefas[j] += X[i][j] * machineJobTime[i][numMachines];
            }
            restricoes.add(C[j] == C[j-1] + I[j-1][numMachines] + tempoTarefas[j]);
        }

        cout << "bloco de restricao 2 adicionado" << endl;

        for(int i = 1; i <= numJobs; ++i){
            for(int j = 1; j <= numJobs; ++j){
                somatorioLinhaX[i] += X[i][j];
                somatorioColunaX[i] += X[j][i];
            }
            restricoes.add(somatorioLinhaX[i] == 1);
            restricoes.add(somatorioColunaX[i] == 1);
            
            restricoes.add(T[i] >= C[i] - deliveryDates[i]);
            restricoes.add(E[i] >= deliveryDates[i] - C[i]);
        }

        cout << "bloco de restricao 3 adicionado" << endl;

        IloArray<IloExprArray> expr4(env, numJobs+1), expr5(env, numJobs+1);
        for(int i = 1; i <= numJobs; ++i){
            expr4[i] = IloExprArray(env, numMachines+1);
            expr5[i] = IloExprArray(env, numMachines+1);
        }
        for(int i = 1; i <= numJobs; ++i){
            for(int j = 1; j <= numMachines; ++j){
                expr4[i][j] = IloExpr(env);
                expr5[i][j] = IloExpr(env);
            }
        }
        for(int j = 1; j < numJobs; ++j){
            for(int k = 1; k < numMachines; ++k){
                for(int i = 1; i <= numJobs; ++i){
                    expr4[j][k] += X[i][j+1] * machineJobTime[i][k];
                    expr5[j][k] += X[i][j] * machineJobTime[i][k+1];
                }
                restricoes.add(I[j][k] + expr4[j][k] + W[j+1][k] == W[j][k] + expr5[j][k] + I[j][k+1]);
            }
        }

        cout << "bloco de restricoes 4 adicionado" << endl;

        cout << "buscando solucao" << endl;
        model.add(restricoes);
        cplex.extract(model);
        cplex.solve();

        //imprimindo o resultado
        cout << "Valor da função objetivo: " << cplex.getObjValue() << endl;
        cout << "Ordem das tarefas: ";
        for(int i = 1; i <= numJobs; ++i){
            for(int j = 1; j <= numJobs; ++j){
                if(cplex.getIntValue(X[j][i])) {
                    cout << j << " \n"[i==numJobs];
                }
            }
        }

        return 0;
    } 
    catch(IloException &e){
        cout << "Erro: " << e << endl;
    }      
    catch(...){
        cout << "Erro: " << endl;
    }
}
