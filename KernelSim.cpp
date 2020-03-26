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
                    cout << "CORE: " << c->id << " TERMINATED PROCESS: " << c->p->id << " /// NOW IDLE" << endl;
                    m.unlock();
                    continue;

                }
                c->p->remaningTime--;
                cout << "CORE: " << c->id << " PROCESSO: " << c->p->id << " TEMPO RESTANTE: " << c->p->remaningTime << endl;
                m.unlock();
                this_thread::sleep_for(chrono::seconds(1));
            }
        }

    }

};


class scheduler {
private:

    vector<process*>* pct;
    vector<unsigned int> readyQueue;
    vector<core*>* corePool;
    unsigned int schedul;
    unsigned int quantum;

public:

    scheduler() {}

    scheduler(vector<process*>* pct, vector<core*>* corePool, unsigned int schedul, unsigned quantum) {
        this->corePool = corePool;
        this->pct = pct;
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

    void scheduleProcess(core* c, process* p) {
        c->p = p;
    }

    process* descheduleProcess(core* c) {
        process* p1 = c->p;
        c->p = nullptr;
        return p1;
    }

    void insertProcess(int pos, int id) {
        readyQueue.insert((readyQueue.cbegin()) + pos, id);
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

            if (pct->size() > readyQueue.size()) {
                m.lock();
                for (int i = readyQueue.size(); i < pct->size(); i++) {
                    process* p = pct->at(i);
                    insertProcess(readyQueue.size(), p->id);
                }
                m.unlock();
            }

            for (core* c : *corePool) {
                if (c->p->state == TERMINATED && readyQueue.empty() == false) {
                    m.lock();
                    descheduleProcess(c);
                    scheduleProcess(c, pct->front());
                    readyQueue.erase(readyQueue.cbegin());
                    pct->erase(pct->cbegin());
                    m.unlock();
                }
            }

            this_thread::sleep_for(chrono::milliseconds(500));
        }

    }

    void algorithmShortestFirst() {

        int* maiorId = new int (0);
        int* maiorTime = new int (0);

        for (int i = 0; i < pct->size(); i++) {
            if (pct->at(i)->totalTime > *maiorTime) {
                *maiorId = pct->at(i)->id;
                *maiorTime = pct->at(i)->totalTime;
            }
        }

        insertProcess(0, *maiorId);

        delete maiorId;
        delete maiorTime;

        while (true) {
            if (pct->size() > readyQueue.size()) {
                m.lock();
                for (int i = readyQueue.size() - 1; i < pct->size(); i++) {
                    for (int j = 0; j < readyQueue.size(); j++) {
                        if (searchFromId(readyQueue.at(j), pct)->totalTime > pct->at(i)->totalTime && isOnList(pct->at(i)->id, readyQueue) == false) {
                            insertProcess(j, pct->at(i)->id);
                            break;
                        }
                    }
                }
                m.unlock();
            }

            for (core* c : *corePool) {
                if (c->p->state == TERMINATED && readyQueue.empty() == false) {
                    m.lock();
                    descheduleProcess(c);
                    for (unsigned int i = 0; i < pct->size(); i++) {
                        if (pct->at(i)->id == readyQueue.front()) {
                            scheduleProcess(c, pct->at(i));
                            readyQueue.erase(readyQueue.cbegin());
                            pct->erase(pct->cbegin() + i);
                            break;
                        }
                    }
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
            if (pct->size() > readyQueue.size()) {
                m.lock();
                for (int i = readyQueue.size(); i < pct->size(); i++) {
                    process* p = pct->at(i);
                    insertProcess(readyQueue.size(), p->id);
                }
                m.unlock();
            }

            for (core* c : *corePool) {
                m.lock();
                if (c->p->state == TERMINATED && readyQueue.empty() == false) {
                    descheduleProcess(c);
                    coreAux.at(c->id) = pct->front()->remaningTime;
                    scheduleProcess(c, pct->front());
                    readyQueue.erase(readyQueue.cbegin());
                    pct->erase(pct->cbegin()); 
                }else if (coreAux.at(c->id) - quantum >= c->p->remaningTime && readyQueue.empty() == false) {
                    process* p = descheduleProcess(c);
                    pct->emplace_back(p);
                    insertProcess(readyQueue.size(),p->id);
                    cout << "QUANTUM SWAP" << endl;
                    coreAux.at(c->id) = pct->front()->remaningTime;
                    scheduleProcess(c, pct->front());
                    readyQueue.erase(readyQueue.cbegin());
                    pct->erase(pct->cbegin());
                }
                m.unlock();
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            

        }
    }

};

class kernel {
private:

    vector<process*>* pct;
    CPU* cpu;
    scheduler schd;
    unsigned int sched;
    unsigned int quantum;

public:
    kernel(CPU* cpu, vector<process*>* pct, unsigned  int sched, unsigned quantum) {
        this->cpu = cpu;
        this->pct = pct;
        this->sched = sched;
        this->quantum = quantum;
        schd = scheduler(pct, &cpu->corePool, sched, quantum);
    };

    void run() {
        thread schThread(&scheduler::run, schd);
        while (true) {this_thread::sleep_for(chrono::milliseconds(500));}
        schThread.join();
    }

    void addProcess(vector<process*> pct1) {
        pct1.emplace_back(new process(id, rand() % 15));
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
    vector<process*> pct;

public:

    simulator(unsigned int coreNumber, unsigned int processInit, unsigned int sched, unsigned int quantum) {
        this->coreNumber = coreNumber;
        this->processInit = processInit;
        this->sched = sched;
        this->quantum = quantum;
        cpu = CPU(coreNumber);
        ker = new kernel(&cpu, &pct, sched, quantum);
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
            if ((rand() % 10 + 1) % 2 == 0 || pct.size() < 2*coreNumber) {
                m.lock();
                for (unsigned int i = 0; i < rand() % 5 + 1; i++)
                {
                    pct.emplace_back(new process(id, rand() % 20 + 1));
                    id++;

                }
                cout << "CREATING RANDOM THREADS" << " process table size: " << pct.size() << endl;
                m.unlock();
            }
            this_thread::sleep_for(chrono::seconds(10));
        }

    };

    void batchProcessInit(unsigned int processInit) {
        for (unsigned int i = 0; i < processInit - 1; i++) {
            pct.emplace_back(new process(id, rand() % 30 + 1));
            id++;
        }
        for (unsigned int i = 0; i < coreNumber; i++)
        {
            cpu.corePool.at(i)->p = pct.front();
            pct.erase(pct.cbegin());
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
