#include <iostream>
#include <list>
#include <thread>

using namespace std;

struct process{
    unsigned int id;
    unsigned int totalTime;
    // 0: READY , 1: RUNNING , -1: TERMINATED
    int state;
    unsigned int remaningTime;
    process(int id, int time) {
        this->id = id;
        this->totalTime = time;
        this->remaningTime = this->totalTime;
        this->state = 0;
    }

};

class core {
    process* p;
    int id;
    core(int id) {
        this->id = id;
    }
};

class CPU {
public:
    int coreNumber;

    CPU(unsigned int corenumber) {
        this->coreNumber = coreNumber;
    }

    void core(int coreId, int processId) {

    }

};

class kernel
{
public:
    void run() {}

    kernel(CPU* cpu) {
        this->cpu = cpu;
    };

private:
    CPU* cpu;
    list<process>* pct = new list<process>;
};

class scheduler {
public:
    list<process>* pct = new list<process>;
    CPU* cpu;

    scheduler(list<process>* pct, CPU* cpu) {
        this->pct = pct;
        this->cpu = cpu;
    }

private:


};

class simulator {
public:

    void run() {
        while (true){




        }
    };

    simulator(unsigned int scheduler, unsigned int processInit, unsigned int quantum, unsigned int coreNumber) {
        this->scheduler = scheduler;
        this->processInit = processInit;
        this->quantum = quantum;
        this->coreNumber = coreNumber;
    }

private:
    unsigned int scheduler = 0;
    unsigned int processInit = 0;
    // 1 ~ 64
    unsigned int coreNumber = 0;
    // 2 ~ 20
    unsigned int quantum = 0;

    CPU* cpu = new CPU(coreNumber);
    thread cpuInstace(CPU(coreNumber));
    kernel ker = kernel(cpu);


    void createRandomProcess() {};
    void batchProcessInit() {};
    void schedulingAlgorithm() {};

};


int main(){
    cout << "SELECIONE O ALGORITMO: 1 - FIFO // 2 - Shortest First // 3 - Round Robin" << "\n";
    unsigned int scheduler = 0;
    cin >> scheduler;

    cout << "QUANTIDADE INICIAL DE PROCESSOS: ";
    unsigned int processInit = 0;
    cin >> processInit;

    cout << "SELECIONE O QUANTUM: ";
    unsigned int quantum = 0;
    cin >> quantum;

    cout << "SELECIONE A QUANTIDADE DE NUCLEOS DO PROCESSADOR: ";
    unsigned int coreNumber = 0;
    cin >> coreNumber;

    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(scheduler, processInit, quantum, coreNumber );
    sim.run();


    return 0;
}