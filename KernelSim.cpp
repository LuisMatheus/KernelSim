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
    bool alive = false;
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
                if (c->alive) {
                    if (c->p->remaningTime == 0) {
                        c->p->state = TERMINATED;
                        c->alive = false;
                        cout << "CORE: " << c->id << " TERMINATED PROCESS: " << c->p->id << "/// NOW IDLE" << endl;
                        continue;

                    }
                    c->p->remaningTime--;
                    cout << "CORE: " << c->id << " TEMPO RESTANTE: " << c->p->remaningTime << endl;
                }
                this_thread::sleep_for(chrono::seconds(1));
            }
        }
        
    }

};

class kernel{
public:
    kernel(CPU* cpu, vector<process*>* pct) {
        this->cpu = cpu;
        this->pct = pct;
    };

    void run() {
        
        while (true){
            for (core* c : cpu->corePool) {
                if (c->p->state == TERMINATED && pct->empty() == false) {
                    m.lock();
                    cout << "DELETING process: " << c->p->id << endl;
                    process* p1 = pct->front();
                    pct->erase(pct->cbegin());
                    delete c->p;
                    c->p = p1;
                    m.unlock();
                }
                this_thread::sleep_for(chrono::milliseconds(500));
            }
            
        }
        
    }

    void addProcess(int id, vector<process*> pct1) {
        pct1.emplace_back(new process(id, rand() % 15));
        cout;
    }

    CPU* cpu;
    vector<process*>* pct;
};

class scheduler {
public:
    vector<process*>* pct;
    vector<int> readyQueue;
    CPU* cpu;
    vector<core*> corePool;
    int schedul = 0;

    scheduler(vector<process*>* pct, CPU* cpu, int schedul) {
        this->cpu = cpu;
        this->pct = pct;
        this->schedul = schedul;
        this->corePool = cpu->corePool;
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
        m.lock();
        c->p = p;
        m.unlock();
    }

    process* descheduleProcess(core* c) {
        process* p1 = c->p;
        c->p = nullptr;
        c->alive = false;
        return p1;
    }

    void insertProcess(int pos, process* p) {

    }

    void run() {
        setScheduleAlgoritihim();
    }

    void algorithmFIFO() {
        /*
        if (pct->size() > readyQueue.size()) {
            for (int i = readyQueue.size(); i < pct->size(); i++) {
                process* p = pct->at(i);
                readyQueue.emplace_back(p->id);
            }
        }
        */
        while (true) {

            for (core* c : corePool) {
                if (c->p->state == TERMINATED) {
                    descheduleProcess(c);
                    //delete c.p.id do pct
                    process* pro = nullptr;
                    for (process* p : *pct) {
                        if (p->id == readyQueue.front()) {
                            pro = p;
                        }
                    }
                    scheduleProcess(c, pro);
                    readyQueue.erase(readyQueue.cbegin());
                }
            }

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
            if ((rand() % 10 + 1) % 2 == 0) {
                for (int i = 0; i < rand() % 5 + 1; i++)
                {
                    pct.emplace_back(new process(rand() % 500000, rand() % 20 + 1));
                    
                }
                cout << "CREATING RANDOM THREADS" << endl;
                cout << "process table size: " << pct.size() << endl;
            }
            this_thread::sleep_for(chrono::seconds(10));
        }
        
    };
    void batchProcessInit(unsigned int processInit) {
        for (int i = 0; i < processInit - 1; i++) {
            pct.emplace_back(new process(rand() % 500000, rand() % 20 + 1));
        }
        for (int i = 0; i < coreNumber; i++)
        {
            cpu.corePool.at(i)->p = pct.front();
            pct.erase(pct.cbegin());
        }
    };
    void schedulingAlgorithm() {};

};


int main(){
    //cout << "SELECIONE O ALGORITMO: 1 - FIFO // 2 - Shortest First // 3 - Round Robin" << "\n";
    unsigned int scheduler = 0;
    //cin >> scheduler;

    cout << "QUANTIDADE INICIAL DE PROCESSOS: ";
    unsigned int processInit = 0;
    cin >> processInit;

    //cout << "SELECIONE O QUANTUM: ";
    unsigned int quantum = 0;
    //cin >> quantum;

    cout << "SELECIONE A QUANTIDADE DE NUCLEOS DO PROCESSADOR: ";
    unsigned int coreNumber = 0;
    cin >> coreNumber;

    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(coreNumber,processInit);
    sim.run();


    return 0;
}
