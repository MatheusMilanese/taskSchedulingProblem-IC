#include <Util.h>

int main(){
    cin >> numJobs >> numMachines;

    machineJobTime.resize(numJobs);
    deliveryDates.resize(numJobs);
    for(int i = 0; i < numJobs; ++i)
        machineJobTime[i].resize(numMachines);

    for(int i = 0; i < numJobs; ++i){
        for(int j = 0; j < numMachines; ++j){
            cin >> machineJobTime[i][j];
        }
    }

    for(int i = 0; i < numJobs; ++i)
        cin >> deliveryDates[i];

    IloEnv env;  // criando o ambiente

    try {
        
    } 
    catch(IloException &e){
        cout << "Erro: " << e << endl;
    }      
    catch(...){
        cout << "Erro: " << endl;
    }

}