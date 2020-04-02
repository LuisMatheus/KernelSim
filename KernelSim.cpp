#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <mutex>
#include <ctime>
#include <cstdlib> 

#define READY 0
#define RUNNING 1
#define TERMINATED -1

using namespace std;

mutex m;

//process id counter
unsigned int id;

struct process {

    unsigned int id;
    unsigned int totalTime;
    int state;
    unsigned int remaningTime;

    process(int id, int time) {
        this->id = id;
        this->totalTime = time;
        this->remaningTime = this->totalTime;
        this->state = READY;
    }

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
    unsigned int coreNumber = 0;
    
    CPU() {}

    CPU(unsigned int coreNumber) {
        for (unsigned int i = 0; i < coreNumber; i++) {
            corePool.emplace_back(new core(i));
        }
    }

    void run() {
        while (true) {
            for (core* c : corePool) {
                m.lock();
                if (c->p == nullptr) {
                    m.unlock();
                    continue;
                }
                if (c->p->state == TERMINATED) {
                    m.unlock();
                    continue;
                }
                if (c->p->remaningTime == 0) {
                    c->p->state = TERMINATED;
                    cout << "CORE: " << c->id << " PROCESSO ENCERRADO: " << c->p->id << endl;
                    m.unlock();
                    continue;

                }
                c->p->remaningTime--;
                cout << "CORE: " << c->id << " PROCESSO: " << c->p->id << " TEMPO RESTANTE: " << c->p->remaningTime << endl;
                m.unlock();
            
            }
            cout << endl;
            this_thread::sleep_for(chrono::seconds(1));
        }

    }

};


class scheduler {
private:

    vector<process*>* readyQueue;
    vector<core*>* corePool;
    unsigned int schedul;
    unsigned int quantum;
    

public:

    scheduler() {}

    scheduler(vector<process*>* readyQueue, vector<core*>* corePool, unsigned int schedul, unsigned quantum) {
        this->readyQueue = readyQueue;
        this->corePool = corePool;
        this->schedul = schedul;
        this->quantum = quantum;
    }

    process* searchFromId(int id, vector<process*>* pct) {
        for (int i = 0; i < pct->size(); i++) {
            if (pct->at(i)->id == id) {
                process* p = pct->at(i);
                return p;
            }
        }
        return NULL;
    }

    bool isOnList(unsigned int id, vector<unsigned int> readyQueue) {
        for (unsigned int i : readyQueue) {
            if (i == id) {
                return true;
            }
        }
        return false;
    }

    void scheduleProcess(process* p, core* c) {
        c->p = p;
    }

    process* descheduleProcess(core* c) {
        process* p1 = c->p;
        c->p = nullptr;
        return p1;
    }

    void insertProcess(unsigned int pos, process* p) {
        readyQueue->insert(readyQueue->cbegin() + pos, p);
    }

    void removeProcess(unsigned int pos) {
        readyQueue->erase(readyQueue->cbegin() + pos);
    }

    void run() {
        setScheduleAlgorithm();
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

        while (true) {

            for (core* c : *corePool) {
                if (c->p->state == TERMINATED && readyQueue->empty() == false) {
                    m.lock();

                    descheduleProcess(c);
                    scheduleProcess(readyQueue->front(), c);
                    removeProcess(0);

                    m.unlock();
                }
            }
            this_thread::sleep_for(chrono::milliseconds(500));
        }

    }

    void algorithmShortestFirst() {

        unsigned int menorProc = UINT_MAX;
        unsigned int indexProc = 0;
        while (true) {

            for (core* c : *corePool) {
                if (c->p->state == TERMINATED && readyQueue->empty() == false) {
                    m.lock();
                    descheduleProcess(c);

                    for (unsigned int i = 0; i < readyQueue->size(); i++) {
                        if (readyQueue->at(i)->totalTime < menorProc) {
                            menorProc = readyQueue->at(i)->totalTime;
                            indexProc = i;
                        }
                    }

                    
                    scheduleProcess(readyQueue->at(indexProc),c);
                    removeProcess(indexProc);

                    menorProc = UINT_MAX;
                    indexProc = 0;

                    m.unlock();
                }

            }

            this_thread::sleep_for(chrono::milliseconds(500));
          
        }
        
    }

    void algorithmRoundRobin() {

        
        vector<int> coreAux;
        for (int i = 0; i < corePool->size(); i++)
        {
            coreAux.emplace_back(corePool->at(i)->p->totalTime);
        }
        
        while (true) {

            for (core* c : *corePool) {
                m.lock();
                if (c->p->state == TERMINATED && readyQueue->empty() == false) {

                    descheduleProcess(c);
                    coreAux.at(c->id) = readyQueue->front()->remaningTime;
                    scheduleProcess(readyQueue->front(), c);
                    removeProcess(0);

                }else if (coreAux.at(c->id) - quantum >= c->p->remaningTime && readyQueue->empty() == false) {

                    process* p = descheduleProcess(c);
                    insertProcess(readyQueue->size(),p);
                    coreAux.at(c->id) = readyQueue->front()->remaningTime;
                    scheduleProcess(readyQueue->front(), c);
                    removeProcess(0);

                }
                m.unlock();
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            

        }
        
    }

};

class kernel {
private:

    CPU* cpu;
    scheduler schd;
    unsigned int sched;
    unsigned int quantum;
    
public:
    vector<process*> pct;


    kernel(CPU* cpu, unsigned  int sched, unsigned quantum) {
        this->cpu = cpu;
        this->sched = sched;
        this->quantum = quantum;
        schd = scheduler(&pct, &cpu->corePool, sched, quantum);
    };

    void run() {
        thread schThread(&scheduler::run, schd);
        while (true) {
            for (int i = 0; i < pct.size() ; i++){
                if (pct.at(i)->state == TERMINATED) {
                    m.lock();
                    killProcess(i);
                    m.unlock();
                }
            }
            this_thread::sleep_for(chrono::milliseconds(500));
        }
        schThread.join();
    }

    void killProcess(int pos) {
        pct.erase(pct.cbegin() + pos);
    }

    void createProcess() {
        pct.emplace_back(new process(id, rand() % 30 + 1));
        id++;
    }


};

class simulator {
private:

    // 1 ~ 64
    unsigned int coreNumber = 0;
    // 2 ~ 20
    unsigned int quantum = 0;
    unsigned int sched;
    unsigned int processInit = 0;
    CPU cpu;
    kernel* ker;

public:

    simulator(unsigned int coreNumber, unsigned int processInit, unsigned int sched, unsigned int quantum) {
        this->coreNumber = coreNumber;
        this->processInit = processInit;
        this->sched = sched;
        this->quantum = quantum;
        cpu = CPU(coreNumber);
        ker = new kernel(&cpu, sched, quantum);
    }

    void run() {
        batchProcessInit(this->processInit);
        thread cput(&CPU::run, cpu);
        this_thread::sleep_for(chrono::milliseconds(50));
        thread kert(&kernel::run, ker);
        while (true) { createRandomProcess(); };
        kert.join();
        cput.join();

    };

    void createRandomProcess() {
        while (true) {
            if ((rand() % 10 + 1) % 2 == 0 || ker->pct.size() < 2*coreNumber) {
                m.lock();
                for (unsigned int i = 0; i < rand() % 5 + 1; i++)
                {
                    ker->createProcess();

                }
                cout << "CREATING RANDOM THREADS" << " process table size: " << ker->pct.size() << endl;
                m.unlock();
                this_thread::sleep_for(chrono::seconds(10));
            }
        }

    };

    void batchProcessInit(unsigned int processInit) {
        for (unsigned int i = 0; i < processInit - 1; i++) {
            ker->createProcess();
        }
        for (unsigned int i = 0; i < coreNumber; i++)
        {
            cpu.corePool.at(i)->p = ker->pct.front();
            ker->killProcess(0);
        }
    };

};


int main() {

    
    cout << "SELECIONE O ALGORITMO: 1 - FIFO // 2 - Shortest First // 3 - Round Robin" << "\n";
    unsigned int scheduler = 2;
    cin >> scheduler;

    cout << "QUANTIDADE INICIAL DE PROCESSOS: ";
    unsigned int processInit = 15;
    cin >> processInit;

    unsigned int quantum = 0;
    if (scheduler == 3) {
        cout << "SELECIONE O QUANTUM: ";
        cin >> quantum;
    }
    
    cout << "SELECIONE A QUANTIDADE DE NUCLEOS DO PROCESSADOR: ";
    unsigned int coreNumber = 5;
    cin >> coreNumber;
    
    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(coreNumber, processInit, scheduler, quantum);
    sim.run();

    return 0;
    
}
