#include <bits/stdc++.h>

using namespace std;

int main() {
    int qtdJobs = 80;
    int qtdMaquinas = 20;
    

    // Abrindo o arquivo para escrita
    ofstream outputFile("");


    // Inicializando a semente para a geração de números aleatórios
    srand(static_cast<unsigned int>(time(nullptr)));

    outputFile << qtdJobs << "\n" << qtdMaquinas << "\n";

    // Gerando n linhas com m números aleatórios
    for (int i = 0; i < qtdJobs; ++i) {
        for (int j = 0; j < qtdMaquinas; ++j) {
            int randomNumber = rand() % 100; // Números aleatórios entre 0 e 99
            outputFile << randomNumber << " ";
        }
        outputFile << "\n"; // Pula para a próxima linha
    }

    // Gerando a última linha com n números aleatórios
    for (int i = 0; i < qtdJobs; ++i) {
        int randomNumber = 100 + (rand() % (100*qtdJobs)); 
        outputFile << randomNumber << " ";
    }

    // Fechando o arquivo
    outputFile.close();

    cout << "Dados gerados e salvos no arquivo." << endl;

    return 0;
}
