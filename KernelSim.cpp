#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <ctime>
#include <cstdlib> 

#define READY 0
#define RUNNING 1
#define TERMINATED -1
#define ABORTED 2

using namespace std;

//process id counter
unsigned int id = 1;

class memoryBlock {
    unsigned int totalBlockSize;
    unsigned int occupiedSize;
    string blockAdress;
    memoryBlock* nextFreeBlock;

};

class process {

public:

    unsigned int id;
    unsigned int totalTime;
    int state;
    unsigned int remaningTime;

    unsigned int totalMemoryUsed;

    vector<memoryBlock*> memoryPointers;

    process(int id, int time) {
        this->id = id;
        this->totalTime = time;
        this->remaningTime = this->totalTime;
        this->state = READY;
    }

    bool generateRandomStaticMemoryCall() {
    
    }

    bool generateRandomDynamicMemoryCall() {
    
    }

};

class memoryManager {
public:
    int allocationAlgorithim;
    unsigned int totalMemory;
    unsigned int memoryOverhead;
    unsigned int occupiedMemory;

    //statistics table
    //quickfit free blocks

    unsigned int minAmountCalls;
    unsigned int numberQuickLists;

    vector<memoryBlock> memory;
    memoryBlock* freeBlocksList;

    memoryManager() {}

    memoryManager(int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls) {
        this->allocationAlgorithim = memAlockAlgo;
        this->totalMemory = totalInstalledMemory;
        this->numberQuickLists = numberQuickList;
        this->minAmountCalls = numberMemoryCalls;
    }

    memoryBlock malloc(unsigned int SIZE) {

    }

    void free(memoryBlock mem) {

    }

    bool checkFreeMemory(unsigned int SIZE) {}

    void setAllocationAlgorithim() {
        switch (allocationAlgorithim){
        case 1:
            firstFit();
            break;
        case 2:
            bestFit();
            break;
        case 3:
            quickFit();
            break;
        default:
            break;
        }
    }

    void firstFit() {}

    void bestFit() {}

    void quickFit() {}

};

class core {
public:

    process* p;
    int id;

    core(int id) {
        this->id = id;
    }
};

class CPU {
public:

    vector<core*> corePool;
    
    CPU() {}

    CPU(unsigned int coreNumber) {
        for (unsigned int i = 0; i < coreNumber; i++) {
            corePool.emplace_back(new core(i));
        }
    }

};

class scheduler {
public:

    vector<process*>* ready_Queue = new vector<process*>();
    vector<process*>* encerrados;
    vector<core*>* corePool;

    vector<int> coreAux;

    unsigned int schedul;
    unsigned int quantum;
  
    scheduler() {}

    scheduler(vector<process*>* encerrados,  vector<core*>* corePool, unsigned int schedul, unsigned int quantum) {
        this->encerrados = encerrados;
        this->corePool = corePool;
        this->schedul = schedul;
        this->quantum = quantum;
    }

    void scheduleProcess(process* p, core* c) {
        p->state = RUNNING;
        c->p = p;
    }

    process* descheduleProcess(core* c) {
        process* p1 = c->p;
        c->p = nullptr;
        return p1;
    }

    process* nextProcess() {
        for (process* p : *ready_Queue) {
            if (p->state == READY) {
                return p;
            }
        }
    }

    void removeProcess(int id) {
        for (int i = 0; i < ready_Queue->size(); i++) {
            if (ready_Queue->at(i)->id == id) {
                ready_Queue->erase(ready_Queue->begin()+i);
            }
        }
    
    }

    void insertionSort(vector<process*>& vetor) {

        for (int i = 1; i < vetor.size(); i++) {
            process* p = vetor[i];
            int escolhido = p->totalTime;
            int j = i - 1;

            while ((j >= 0) && (vetor[j]->totalTime > escolhido)) {
                vetor[j + 1] = vetor[j];
                j--;
            }

            vetor[j + 1] = p;
        }
    }

    void run() {
        
        while (true) {

            cout << "CPU:" << endl;
            cout << endl;
            for (core* c : *corePool) {

                if (c->p == nullptr) {
                    process* p1 = nextProcess();
                    scheduleProcess(p1, c);
                }
                

                c->p->remaningTime--;

                if (c->p->remaningTime == 0) {
                    c->p->state = TERMINATED;
                    cout << "CORE: " << c->id << " PROCESSO ENCERRADO: " << c->p->id << "\t";
                    continue;
                    
                }

                
                
                cout << "CORE: " << c->id << " P: " << c->p->id << " TR: " << c->p->remaningTime << "\t";
            }
            
            
            cout << endl << endl << endl;

            cout << "PROCESSOS:" << endl;
            cout << endl;

            for(process* p : *ready_Queue){
                cout << "{[ID:" << p->id << "] , [TT:" << p->totalTime << "] , [TR:" << p->remaningTime <<"] , [STATUS:" <<p->state << "]}" <<"\n";
            }

            cout << endl;
            cout << endl;
            cout << endl;

            
            cout << "ENCERRADOS:" << endl;
            cout << endl;

            for (process* p : *encerrados) {
                cout << "ID: " << p->id << "\t";;
            }

            cout << endl;
            cout << endl;
            cout << endl;
            

            this_thread::sleep_for(chrono::seconds(2));

            setScheduleAlgorithm();

        }

        
    }

    void setScheduleAlgorithm() {
        switch (schedul) {
        case 1:
            algorithmFIFO();
            break;
        case 2:
            algorithmShortestFirst();
            break;
        case 3:
            algorithmRoundRobin();
            break;
        default:
            break;
        }
    }

    void algorithmFIFO() {

        cout << "\n" << "========================================================" << "\n";

        for (core* c : *corePool) {
            if (c->p->state == TERMINATED) {

                process* p1 = nextProcess();
                descheduleProcess(c);
                scheduleProcess(p1,c);
                

            }
        }
        

    }

    void algorithmShortestFirst() {

        cout << "\n" << "========================================================" << "\n";

        insertionSort(*ready_Queue);

        for (core* c : *corePool) {
            if (c->p->state == TERMINATED) {
                
                process* p1 = nextProcess();
                descheduleProcess(c);
                scheduleProcess(p1, c);

            }

        }

    }

    void roundRobinAux(vector<process*>& vetor) {
        process* p = vetor[0];
        for (int i = 0; i < ready_Queue->size() - 1; i++) {

            vetor[i] = vetor[i + 1];
        }
        vetor[ready_Queue->size() - 1] = p;
    
    }

    void algorithmRoundRobin() {

        if (coreAux.size() == 0) {
            for (int i = 0; i < corePool->size(); i++) {

                coreAux.emplace_back(corePool->at(i)->p->totalTime);

            }
        }
        
        cout << "\n" << "========================================================" << "\n";

        for (core* c : *corePool) {
            if (c->p->state == TERMINATED) {

                process* p1 = nextProcess();
                coreAux.at(c->id) = p1->remaningTime;
                descheduleProcess(c);
                scheduleProcess(p1, c);

            }
            if (c->p->remaningTime <= coreAux.at(c->id) - quantum) {

                c->p->state = READY;
                descheduleProcess(c);

                roundRobinAux(*ready_Queue);
                
                process* p = nextProcess();

                coreAux.at(c->id) = p->remaningTime;
                scheduleProcess(p , c);
                

            }
            
        }
            
    }

};

class kernel {
private:

    CPU* cpu;
    scheduler schd;
    memoryManager memMan;
    
public:

    vector<process*> pct;

    vector<process*> encerrados;

    kernel(CPU* cpu, unsigned  int sched, unsigned quantum , int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls) {
        this->cpu = cpu;
        schd = scheduler(&encerrados , &cpu->corePool, sched, quantum);
        memMan = memoryManager(memAlockAlgo, totalInstalledMemory, numberQuickList, numberMemoryCalls);
    };

    void run() {
        thread schThread(&scheduler::run, schd);
        this_thread::sleep_for(chrono::seconds(1));
        Thread memThread(&memoryManager::)
        while (true) {

            for (process* p : pct) {
                if (p->state == TERMINATED) {
                    killProcess(p->id);
                }
            }

            this_thread::sleep_for(chrono::milliseconds(500));
        }
        schThread.join();
    }

    void killProcess(int id) {
        for (int i = 0; i < pct.size(); i ++) {
            if (pct.at(i)->id == id) {
                encerrados.emplace_back(pct.at(i));
                pct.erase(pct.begin() + i);
                schd.removeProcess(id);
            }
        }
    }

    void createProcess() {
        process* p = new process(id, rand() % 30 + 1);
        pct.emplace_back(p);
        schd.ready_Queue->emplace_back(p);
        id++;
    }

    void memoryAllocation() {}

    void freeMemory() {}

};

class simulator {
private:

    unsigned int processInit;
    CPU cpu;
    kernel* ker;

public:

    simulator(unsigned int coreNumber, unsigned int processInit, unsigned int sched, unsigned int quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls) {
        this->processInit = processInit;
        cpu = CPU(coreNumber);
        ker = new kernel(&cpu, sched, quantum, memAlockAlgo, totalInstalledMemory, numberQuickList, numberMemoryCalls);
    }

    void run() {
        batchProcessInit(this->processInit);
        this_thread::sleep_for(chrono::milliseconds(500));
        thread kert(&kernel::run, ker);
        while (true) {
            createRandomProcess();
            this_thread::sleep_for(chrono::seconds(10));
        };
        kert.join();

    };

    void createRandomProcess() {
            if ((rand() % 10 + 1) % 2 == 0) {
                for (unsigned int i = 0; i < rand() % 5 + 1; i++)
                {
                    
                    ker->createProcess();

                }
                //cout << "CREATING RANDOM THREADS" << " process table size: " << ker->pct.size() << endl;
            }
    };

    void batchProcessInit(unsigned int processInit) {
        for (unsigned int i = 0; i < processInit - 1; i++) {

            ker->createProcess();

        }
    };

};

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
