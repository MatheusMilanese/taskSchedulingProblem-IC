#include <Util.h>

int main(){
    /*--------------------------------------------------*
     *       L E I T U R A   D A   E N T R A D A        *
     *--------------------------------------------------*/
    cin >> numJobs >> numMachines;

    machineJobTime.resize(numJobs+1);
    deliveryDates.resize(numJobs+1);
    for(int i = 1; i <= numJobs; ++i)
        machineJobTime[i].resize(numMachines+1);

    for(int i = 1; i <= numJobs; ++i){
        for(int j = 1; j <= numMachines+1; ++j){
            cin >> machineJobTime[i][j];
        }
    }

    for(int i = 1; i <= numJobs; ++i)
        cin >> deliveryDates[i];

    /*--------------------------------------------------*
     *        C R I A N D O    O    M O D E L O         *
     *--------------------------------------------------*/
    IloEnv env;  // criando o ambiente

    try {
        IloModel model(env, "Modelo"); // criando o modelo
        IloCplex cplex(env); //criando o cplex

        // Variaveis de decisao
        BoolVarMatrix X(env, numJobs+1);    // Ordem de realização dos "jobs"
        IloNumVarArray C(env, numJobs+1);   // Tempo em que o "job" foi terminado
        IloNumVarArray E(env, numJobs+1);   // Adiantamento da entrega do "job"
        IloNumVarArray T(env, numJobs+1);   // Atraso na entrega do "Job"

        NumVarMatrix2D W(env, numJobs+1);
        NumVarMatrix2D I(env, numJobs);
        IloNumVar I0;

        for(int i = 1; i <= numJobs; ++i)
            X[i] = IloBoolVarArray(env, numJobs+1);
    
        for(int i = 1; i < numJobs; ++i) {
            W[i] = IloNumVarArray(env, numMachines);
            I[i] = IloNumVarArray(env, numMachines+1);
        }
        W[numJobs] = IloNumVarArray(env, numMachines);

        // Funçao Objetivo
        IloExpr Objetivo(env);
        for(int i = 1; i <= numJobs; ++i)
            Objetivo += (E[i] + T[i]);
        
        model.add(IloMinimize(env, Objetivo));

        // Restricoes
    } 
    catch(IloException &e){
        cout << "Erro: " << e << endl;
    }      
    catch(...){
        cout << "Erro: " << endl;
    }
}
