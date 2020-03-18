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
unsigned int id;

struct process{
    unsigned int id;
    unsigned int totalTime;
    int state;
    unsigned int remaningTime;
    process(int id, int time) {
        this->id = id;
        this->totalTime = time;
        this->remaningTime = this->totalTime;
        this->state = RUNNING;
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
    for (unsigned int i = 0; i < coreNumber; i++){
        corePool.emplace_back(new core(i));
    }
    }

    void run() {
        while (true){
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
                cout << "CORE: " << c->id << " ID PROCESSO: " << c->p->id << " TEMPO RESTANTE: " << c->p->remaningTime << endl;
                m.unlock();
                this_thread::sleep_for(chrono::seconds(1));
            }
        }
        
    }

};


class scheduler {
public:
    vector<process*>* pct;
    vector<unsigned int> readyQueue;
    vector<core*>* corePool;
    unsigned int schedul = 0;

    scheduler() {}

    scheduler(vector<process*>* pct, vector<core*>* corePool, unsigned int schedul) {
        this->corePool = corePool;
        this->pct = pct;
        this->schedul = schedul;
    }

    void setScheduleAlgoritihim() {
        switch (schedul) {
        case 0:
            algorithmFIFO();
            break;
        case 1:
            algorithmShortestFirst();
            break;
        case 2:
            algorithmRoundRobin();
            break;
        default:
            break;
        }
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
        readyQueue.insert((readyQueue.cbegin())+pos,id);
    }

    void run() {
        setScheduleAlgoritihim();
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
                if (c->p == nullptr) {
                    m.lock();
                    scheduleProcess(c, pct->front());
                    pct->erase(pct->cbegin());
                    readyQueue.erase(readyQueue.cbegin());
                    m.unlock();
                }else if (c->p->state == TERMINATED) {
                    m.lock();
                    descheduleProcess(c);
                    for (unsigned int i = 0; i < pct->size(); i++) {
                        if (pct->at(i)->id == readyQueue.front()) {
                            scheduleProcess(c,pct->at(i));
                            readyQueue.erase(readyQueue.cbegin());
                            pct->erase(pct->cbegin()+i);
                            break;
                        }
                    }
                    m.unlock();
                    
                }
            }
            this_thread::sleep_for(chrono::milliseconds(200));
        }

    }


    void algorithmShortestFirst() {
        while (true) {

        }
    }

    void algorithmRoundRobin() {
        while (true) {

        }
    }


};

class kernel {
public:
    kernel(CPU* cpu, vector<process*>* pct) {
        this->cpu = cpu;
        this->pct = pct;
        schd = scheduler(pct, &cpu->corePool, 0);
    };

    void run() {
        thread schThread(&scheduler::run, schd);
        while (true) {
             this_thread::sleep_for(chrono::milliseconds(500));
        }
        schThread.join();
    }

    void addProcess(vector<process*> pct1) {
        pct1.emplace_back(new process(id, rand() % 15));
        id++;
    }

    CPU* cpu;
    vector<process*>* pct;
    scheduler schd;
};

class simulator {
public:

    simulator(unsigned int coreNumber,unsigned int processInit) {
        this->coreNumber = coreNumber;
        this->processInit = processInit;
        cpu = CPU(coreNumber);
    }

    void run() {
        batchProcessInit(this->processInit);
        thread cput(&CPU::run, cpu);
        this_thread::sleep_for(chrono::seconds(2));
        thread kert(&kernel::run, ker);
        while (true) { createRandomProcess(); };
        kert.join();
        cput.join();
        
    };

    unsigned int scheduler = 0;

    unsigned int processInit = 0;
    // 1 ~ 64
    unsigned int coreNumber = 0;
    // 2 ~ 20
    unsigned int quantum = 0;

    CPU cpu;
    kernel* ker = new kernel(&cpu,&pct);
    vector<process*> pct;
    

    void createRandomProcess() {
        while (true){
            m.lock();
            if ((rand() % 10 + 1) % 2 == 0) {
                for (unsigned int i = 0; i < rand() % 5 + 1; i++)
                {
                    pct.emplace_back(new process(id, rand() % 20 + 1));
                    id++;
                    
                }
                cout << "CREATING RANDOM THREADS" << " process table size: " << pct.size() << endl;
            }
            m.unlock();
            this_thread::sleep_for(chrono::seconds(5));
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


int main(){
    //cout << "SELECIONE O ALGORITMO: 1 - FIFO // 2 - Shortest First // 3 - Round Robin" << "\n";
    unsigned int scheduler = 0;
    //cin >> scheduler;

    cout << "QUANTIDADE INICIAL DE PROCESSOS: ";
    unsigned int processInit = 15;
    //cin >> processInit;

    //cout << "SELECIONE O QUANTUM: ";
    unsigned int quantum = 0;
    //cin >> quantum;

    cout << "SELECIONE A QUANTIDADE DE NUCLEOS DO PROCESSADOR: ";
    unsigned int coreNumber = 5;
    //cin >> coreNumber;

    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(coreNumber,processInit);
    sim.run();


    return 0;
}
