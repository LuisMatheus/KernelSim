#include <iostream>

#include "Simulador.h"

using namespace std;

int main() {

    /*
    cout << "SELECIONE O ALGORITIMO: 1 - FIFO // 2 - Shortest First // 3 - Round Robin" << endl;
    unsigned int scheduler = 3;
    cin >> scheduler;

    cout << "QUANTIDADE INICIAL DE PROCESSOS: " << endl;
    unsigned int processInit = 15;
    cin >> processInit;

    unsigned int quantum = 2;

    if (scheduler == 3) {
        cout << "SELECIONE O QUANTUM: " << endl;
        cin >> quantum;
    }

    cout << "SELECIONE A QUANTIDADE DE NUCLEOS DO PROCESSADOR: " << endl;
    unsigned int coreNumber = 2;
    cin >> coreNumber;
    
    cout << "SELECIONE O ALGORITIMO DE ALOCACAO DE MEMORIA: 1 - FIRST FIT / 2 - BEST FIT / 3 - QUICK FIT" << endl;
    unsigned int memLockAlgo = 1;
    cin >> memLockAlgo;

    cout << "MEMORIA INSTALADA" << endl;
    unsigned int totalInstaledMemory = 20000;
    cin >> totalInstaledMemory;

    unsigned int numberQuickList = 0;
    unsigned int numberMemoryCalls = 0;
    if (memLockAlgo == 3) {
        cout << "NUMERO DE LISTAS UILIZADAS PELO QUICKFIT" << endl;
        cin >> numberQuickList;
        cout << "NUMERO DE CHAMADAS PARA CRIAR LISTA PELO QUICKFIT" << endl;
        cin >> numberMemoryCalls;
    }
    
    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(coreNumber, processInit, scheduler , quantum, memLockAlgo, totalInstaledMemory, numberQuickList, numberMemoryCalls);
    */

    simulator sim = simulator(2, 30, 1, 2, 3, 10000, 4, 20);
    sim.run();

    return 0;

}