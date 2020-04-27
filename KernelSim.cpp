#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <ctime>
#include <cstdlib> 

#define READY 0
#define RUNNING 1
#define TERMINATED -1

using namespace std;

//process id counter
unsigned int id = 1;

class process {

public:

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
    
    CPU() {}

    CPU(unsigned int coreNumber) {
        for (unsigned int i = 0; i < coreNumber; i++) {
            corePool.emplace_back(new core(i));
        }
    }

};


class scheduler {
private:

    vector<process*>* pct;
    vector<process*>* ready_Queue = new vector<process*>();
    vector<process*>* encerrados;
    vector<core*>* corePool;

    vector<int> coreAux;

    unsigned int schedul;
    unsigned int quantum;
    

public:

    scheduler() {}

    scheduler(vector<process*>* pct, vector<process*>* encerrados,  vector<core*>* corePool, unsigned int schedul, unsigned int quantum) {
        this->pct = pct;
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


    void removeProcess(unsigned int p) {
        for (int i = 0; i < ready_Queue->size(); i++) {
            if (p == ready_Queue->at(i)->id) {
                ready_Queue->erase(ready_Queue->begin() + i);
            }
        }
    
    }


    void insertProcess(unsigned int pos, process* p) {
        p->state = READY;
        ready_Queue->insert(ready_Queue->begin() + pos, p);
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

    void queueCheck() {
        if (ready_Queue->size() < pct->size()) {
                       
            for (int i = ready_Queue->size(); i < pct->size(); i++) {

                insertProcess(i, pct->at(i));

            }

            if (schedul == 2) {
                insertionSort(*ready_Queue);
            }
       
        }

    }

    void run() {
        
        while (true) {

            queueCheck();

            cout << "CPU:" << endl;
            cout << endl;
            for (core* c : *corePool) {
                if (c->p == nullptr) {
                    continue;

                }
                

                c->p->remaningTime--;

                if (c->p->remaningTime == 0) {
                    c->p->state = TERMINATED;
                    removeProcess(c->p->id);
                    cout << "CORE: " << c->id << " PROCESSO ENCERRADO: " << c->p->id << "\t";
                    continue;
                    
                }

                
                
                cout << "CORE: " << c->id << " P: " << c->p->id << " TR: " << c->p->remaningTime << "\t";
            }
            
            
            cout << endl << endl << endl << endl << endl;

            cout << "PROCESSOS:" << endl;
            cout << endl;

            for(process* p : *ready_Queue){
               // cout << "{[ID: " << p->id << "] , [TT: " << p->totalTime << "] , [TR: " << p->remaningTime << "]}" <<"\t";
                cout << p->id << " , " <<p->remaningTime <<endl;
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
            if (c->p->state == TERMINATED && pct->empty() == false) {

                process* p1 = nextProcess();
                descheduleProcess(c);
                scheduleProcess(p1,c);
                

            }
        }
        

    }

    void algorithmShortestFirst() {

        cout << "\n" << "========================================================" << "\n";

        for (core* c : *corePool) {
            if (c->p->state == TERMINATED && pct->empty() == false) {
                
                process* p1 = nextProcess();
                descheduleProcess(c);
                scheduleProcess(p1, c);

            }

        }

          
        
        
    }

    void algorithmRoundRobin() {

       
        if (coreAux.size() == 0) {
            for (int i = 0; i < corePool->size(); i++) {

                coreAux.emplace_back(corePool->at(i)->p->totalTime);

            }
        }
        
        cout << "\n" << "========================================================" << "\n";

        for (core* c : *corePool) {
            if (c->p->state == TERMINATED && pct->empty() == false) {
                //TODO
                process* p1 = nextProcess();
                pct->emplace_back(descheduleProcess(c));
                coreAux.at(c->id) = p1->remaningTime;
                scheduleProcess(p1, c);
                //removeProcess(p1);

            }
            if (c->p->remaningTime <= coreAux.at(c->id) - quantum && pct->empty() == false) {

                cout << endl << "swap: " << c->p->id << endl << endl;
                //TODO
                process* p = descheduleProcess(c);
                p->state = READY;
                insertProcess(pct->size(),p);
                process* p1 = nextProcess();
                coreAux.at(c->id) = p1->remaningTime;
                scheduleProcess(p1, c);
                //removeProcess(p1);

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

    vector<process*> encerrados;

    kernel(CPU* cpu, unsigned  int sched, unsigned quantum) {
        this->cpu = cpu;
        this->sched = sched;
        this->quantum = quantum;
        schd = scheduler(&pct, &encerrados , &cpu->corePool, sched, quantum);
    };

    void run() {
        thread schThread(&scheduler::run, schd);
        this_thread::sleep_for(chrono::seconds(1));
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
            }
        }
    }

    void createProcess() {
        pct.emplace_back(new process(id, rand() % 30 + 1));
        id++;
    }

};

class simulator {
private:

    // 1 ~ 64
    unsigned int coreNumber;
    // 2 ~ 20
    unsigned int quantum;
    unsigned int sched;
    unsigned int processInit;
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
                for (unsigned int i = 0; i < rand() % 3 + 1; i++)
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
        for (unsigned int i = 0; i < coreNumber; i++) {

            process* p1 = ker->pct.at(i);
            p1->state = RUNNING;
            cpu.corePool.at(i)->p = p1;

        }
    };

};


int main() {

    
    //cout << "SELECIONE O ALGORITIMO: 1 - FIFO // 2 - Shortest First // 3 - Round Robin" << "\n";
    unsigned int scheduler = 2;
    //cin >> scheduler;

    //cout << "QUANTIDADE INICIAL DE PROCESSOS: ";
    unsigned int processInit = 10;
    //cin >> processInit;

    unsigned int quantum = 2;
    /*
    if (scheduler == 3) {
        cout << "SELECIONE O QUANTUM: ";
        cin >> quantum;
    }
    */
    
    //cout << "SELECIONE A QUANTIDADE DE NUCLEOS DO PROCESSADOR: ";
    unsigned int coreNumber = 1;
    //cin >> coreNumber;
    
    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(coreNumber, processInit, scheduler, quantum);
    sim.run();

    return 0;
    
}
