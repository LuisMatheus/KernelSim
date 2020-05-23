#include <iostream>

#include"simulador.h"

using namespace std;

int main() {

    
    cout << "SELECIONE O ALGORITIMO: 1 - FIFO // 2 - Shortest First // 3 - Round Robin" << "\n";
    unsigned int scheduler = 3;
    cin >> scheduler;

    cout << "QUANTIDADE INICIAL DE PROCESSOS: ";
    unsigned int processInit = 15;
    cin >> processInit;

    unsigned int quantum = 2;
    
    if (scheduler == 3) {
        cout << "SELECIONE O QUANTUM: ";
        cin >> quantum;
    }
    
    
    cout << "SELECIONE A QUANTIDADE DE NUCLEOS DO PROCESSADOR: ";
    unsigned int coreNumber = 2;
    cin >> coreNumber;
    
    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(coreNumber, processInit, scheduler, quantum,1,999999,15,25);
    sim.run();

    return 0;
    
}
