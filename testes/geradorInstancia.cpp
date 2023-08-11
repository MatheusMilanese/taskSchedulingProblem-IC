#include <bits/stdc++.h>

int main() {
    int qtdJobs = 10;
    int qtdMaquinas = 20;
    

    // Abrindo o arquivo para escrita
    std::ofstream outputFile("n100m20/instancia6");


    // Inicializando a semente para a geração de números aleatórios
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    outputFile << qtdJobs << "\n" << qtdMaquinas << "\n";

    // Gerando n linhas com m números aleatórios
    for (int i = 0; i < qtdJobs; ++i) {
        for (int j = 0; j < qtdMaquinas; ++j) {
            int randomNumber = std::rand() % 100; // Números aleatórios entre 0 e 99
            outputFile << randomNumber << " ";
        }
        outputFile << "\n"; // Pula para a próxima linha
    }

    // Gerando a última linha com n números aleatórios
    for (int i = 0; i < qtdJobs; ++i) {
        int randomNumber = 100 + (std::rand() % (100*qtdJobs)); // Números aleatórios entre 0 e 99
        outputFile << randomNumber << " ";
    }

    // Fechando o arquivo
    outputFile.close();

    std::cout << "Dados gerados e salvos no arquivo 'output.txt'." << std::endl;

    return 0;
}
