#include <iostream>
#include <list>
#include <thread>
#include <vector>
#include <ctime>
#include <cstdlib> 
#include <mutex>

#define READY 0
#define RUNNING 1
#define TERMINATED -1
#define ABORTED 2

#define BLOCK_SIZE 16

using namespace std;

mutex mu;

class memoryBlock {
public:
    unsigned int totalBlockSize;
    unsigned int occupiedSize;
    unsigned int blockAdress;
    memoryBlock* nextFreeBlock;

    memoryBlock(unsigned int totalBlockSize, unsigned int blockAdress) {
        this->totalBlockSize = totalBlockSize;
        this->occupiedSize = totalBlockSize;
        this->blockAdress = blockAdress;
    }
};

class process {

public:

    unsigned int id;
    unsigned int totalTime;
    int state;
    unsigned int remaningTime;

    unsigned int totalMemoryUsed;

    vector<memoryBlock*> memoryPointer;

    process(int id, int time, unsigned int totalMemoryUsed) {
        this->id = id;
        this->totalTime = time;
        this->remaningTime = this->totalTime;
        this->state = READY;
        this->totalMemoryUsed = totalMemoryUsed;

    }

    void generateRandomStaticMemoryCall(memoryBlock* adress) {
        memoryPointer.emplace_back(adress);
    }

    void generateRandomDynamicMemoryCall(memoryBlock* m) {
        totalMemoryUsed += m->occupiedSize;
        memoryPointer.emplace_back(m);
    }

};

class memoryManager {
public:
    int allocationAlgorithim = 0;
    unsigned int totalMemory = 0;
    unsigned int memoryOverhead = 0;
    unsigned int occupiedMemory = 0;

    //statistics table
    //quickfit free blocks
    //quickfit special blocks

    unsigned int minAmountCalls;
    unsigned int numberQuickLists;

    unsigned int adrCounter = 1;

    vector<memoryBlock*> memory;
    memoryBlock* freeBlocksList = nullptr;

    memoryManager() {}

    memoryManager(int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls) {
        this->allocationAlgorithim = memAlockAlgo;
        this->totalMemory = totalInstalledMemory;
        this->numberQuickLists = numberQuickList;
        this->minAmountCalls = numberMemoryCalls;
    }

    int getAvailableMemory(){
        return totalMemory - (memoryOverhead + occupiedMemory);
    }

    memoryBlock* malloc(unsigned int SIZE) {
        if (checkFreeMemory(SIZE)) {
            return setAllocationAlgorithim(SIZE);
        }
        else {
            return nullptr;
        }
    }

    void free(memoryBlock* mem) {
        occupiedMemory -= mem->occupiedSize;
        mem->occupiedSize = 0;
        mem->nextFreeBlock = nullptr;
        memoryBlock* m = freeBlocksList;
        if (m != nullptr) {
            while (m->nextFreeBlock != nullptr) {
                m = m->nextFreeBlock;
            }
            m->nextFreeBlock = mem;
        }
        else {
            freeBlocksList = mem;
        }
    }

    bool checkFreeMemory(unsigned int SIZE) {
        if (occupiedMemory - totalMemory >= SIZE) {
            return true;
        }
        else {
            memoryBlock* nextBl = freeBlocksList;
            while (nextBl != nullptr) {
                if (nextBl->totalBlockSize >= SIZE ) {
                    return true;
                }
                nextBl = nextBl->nextFreeBlock;
            }
            return false;
        }
    }

    memoryBlock* setAllocationAlgorithim(unsigned int SIZE) {
        switch (allocationAlgorithim) {
        case 1:
            return firstFit(SIZE);
        case 2:
            return bestFit(SIZE);
        case 3:
            return quickFit(SIZE);
        default:
            return NULL;
        }
    }

    memoryBlock* firstFit(unsigned int SIZE) {
        memoryBlock* m;
        m = freeBlocksList;

        while (m != nullptr) {

            if (m->totalBlockSize >= SIZE) {

                if (m == freeBlocksList) {
                    m->occupiedSize = SIZE;
                    occupiedMemory += SIZE;
                    freeBlocksList = m->nextFreeBlock;
                    m->nextFreeBlock = nullptr;
                    return m;
                }

                memoryBlock* aux = freeBlocksList;

                while (aux->nextFreeBlock->blockAdress != m->blockAdress) {
                    aux = aux->nextFreeBlock;
                }
                aux->nextFreeBlock = m->nextFreeBlock;
                occupiedMemory += SIZE;
                m->occupiedSize = SIZE;
                m->nextFreeBlock = nullptr;
                return m;
            }
            m = m->nextFreeBlock;
        }
        mu.lock();
        if (getAvailableMemory() - SIZE >= 0) {
            occupiedMemory += SIZE;
            memoryOverhead += BLOCK_SIZE;
            m = new memoryBlock(SIZE, adrCounter++);
            memory.emplace_back(m);
            mu.unlock();
            return m;
        }
       
        else {
            mu.unlock();
            return nullptr;
        }
        
        
    }

    memoryBlock* bestFit(unsigned int SIZE) { 
        memoryBlock* m = freeBlocksList;
        
        int count = INT32_MAX;
        memoryBlock* bf = nullptr;
        int test;
        while (m != nullptr) {
            test = m->totalBlockSize - SIZE;
            if (test >= 0 && test <= count ) {
                count = test;
                bf = m;
            }
            m = m->nextFreeBlock;
        }

        if (bf != nullptr) {

            if (bf == freeBlocksList) {
                bf->occupiedSize = SIZE;
                occupiedMemory += SIZE;
                freeBlocksList = bf->nextFreeBlock;
                bf->nextFreeBlock = nullptr;
                return bf;
            }
            else {
                m = freeBlocksList;
                while (m->nextFreeBlock->blockAdress != bf->blockAdress) {
                    m = m->nextFreeBlock;
                }

                m->nextFreeBlock = bf->nextFreeBlock;
                occupiedMemory += SIZE;
                bf->occupiedSize = SIZE;
                bf->nextFreeBlock = nullptr;
                return bf;
            }

        }
        
        mu.lock();
        if (bf == nullptr && getAvailableMemory() - SIZE >= 0) {
            occupiedMemory += SIZE;
            memoryOverhead += BLOCK_SIZE;
            m = new memoryBlock(SIZE, adrCounter++);
            memory.emplace_back(m);
            mu.unlock();
            return m;
        }else {
            mu.unlock();
            return nullptr;
        }
    }

    memoryBlock* quickFit(unsigned int SIZE) { return NULL; }

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

    scheduler(vector<process*>* encerrados, vector<core*>* corePool, unsigned int schedul, unsigned int quantum) {
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
                ready_Queue->erase(ready_Queue->begin() + i);
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

            for (core* c : *corePool) {

                if (c->p == nullptr) {
                    process* p1 = nextProcess();
                    scheduleProcess(p1, c);
                }

                c->p->remaningTime--;

                if (c->p->remaningTime == 0) {
                    c->p->state = TERMINATED;
                    continue;

                }

            }

            setScheduleAlgorithm();

            this_thread::sleep_for(chrono::seconds(2));
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

        for (core* c : *corePool) {
            if (c->p->state == TERMINATED) {

                process* p1 = nextProcess();
                descheduleProcess(c);
                scheduleProcess(p1, c);


            }
        }


    }

    void algorithmShortestFirst() {

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
                scheduleProcess(p, c);


            }

        }

    }

};

class kernel {
private:

    CPU* cpu;
    scheduler schd;
    memoryManager memMan;

    unsigned int id = 1;

public:

    vector<process*> pct;

    vector<process*> encerrados;

    kernel(CPU* cpu, unsigned  int sched, unsigned quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls) {
        this->cpu = cpu;
        schd = scheduler(&encerrados, &cpu->corePool, sched, quantum);
        memMan = memoryManager(memAlockAlgo, totalInstalledMemory, numberQuickList, numberMemoryCalls);
    };

    void run() {
        thread schThread(&scheduler::run, schd);
        this_thread::sleep_for(chrono::milliseconds(500));
        while (true) {

            cout << "PROCESSOS:" << endl;

            for (process* p : *schd.ready_Queue) {
                cout << "{[ID:" << p->id << "] , [TT:" << p->totalTime << "] , [TR:" << p->remaningTime << "] , [STATUS:" << p->state << "] , [MEMORY:" << p->totalMemoryUsed <<"]}" << "\n";
            }

            cout << endl;

            cout << "MEMORIA:" << endl;
            cout << "TOTAL: " << memMan.totalMemory << "\t" << "OCUPADO: " << memMan.occupiedMemory + memMan.memoryOverhead << "\t" <<"DISPONIVEL: " << memMan.getAvailableMemory() << endl;
            for (memoryBlock* m : memMan.memory) {
                cout << "[" << m->blockAdress << "," << m->totalBlockSize << "," << m->occupiedSize << "]" << " ";
            }

            cout << endl << endl;

            cout << "ENCERRADOS:" << endl;

            for (process* p : encerrados) {
                cout << "[ID: " << p->id <<","<<p->state <<"]  ";;
            }

            cout << endl;
            cout << endl;
            cout << endl;



            for (process* p : pct) {
                                
                if (p->state == TERMINATED) {
                    killProcess(p->id);
                }

                if (rand() % 100 + 1 <= 5) {
                    memoryBlock* m = memoryAllocation(rand() % 200 + 1);
                    if (m!=nullptr) {
                        p->generateRandomDynamicMemoryCall(m);
                    }
                    else {
                        cout << "OUT OF MEMORY: FAILED DYNAMIC CALL";
                    }
                    
                }
            }

            this_thread::sleep_for(chrono::seconds(2));
        }
        schThread.join();
    }

    void killProcess(int id) {
        for (int i = 0; i < pct.size(); i++) {
            if (pct.at(i)->id == id) {
                process* p = pct.at(i);
                encerrados.emplace_back(p);
                freeMemory(p);
                pct.erase(pct.begin() + i);
                schd.removeProcess(id);
            }
        }
    }

    void createProcess() {
        //todo
        process* p = new process(id++, rand() % 30 + 1, 2^((rand() % 10) + 1));
        p->generateRandomStaticMemoryCall(memoryAllocation(p->totalMemoryUsed));
        if (p->memoryPointer.at(0) != nullptr) {
            pct.emplace_back(p);
            schd.ready_Queue->emplace_back(p);
        }
        else {
            p->state = ABORTED;
            cout << "||||||SYSTEM OUT OF MEMORY ABORTING PROCESS: " << p->id;
            encerrados.emplace_back(p);
        }
              
    }

    memoryBlock* memoryAllocation(unsigned int SIZE) {
        if (memMan.checkFreeMemory(SIZE)) {
            return memMan.malloc(SIZE);
        }
        return nullptr;
        
    }

    void freeMemory(process* p) {
        for (memoryBlock* m : p->memoryPointer) {
            memMan.free(m);
        }
        p->memoryPointer.clear();
    }

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
            this_thread::sleep_for(chrono::seconds(8));
        };
        kert.join();

    };

    void createRandomProcess() {
        if ((rand() % 10 + 1) % 2 == 0) {
            for (unsigned int i = 0; i < rand() % 5 + 1; i++)
            {

                ker->createProcess();

            }
        }
    };

    void batchProcessInit(unsigned int processInit) {
        for (unsigned int i = 0; i < processInit - 1; i++) {

            ker->createProcess();

        }
    };

};

int main() {

    /*
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
    */
    cout << "inicializando Simulacao" << "\n" << "========================================================" << "\n";
    simulator sim = simulator(1, 20, 1, 2, 1, 20000, 15, 25);
    sim.run();

    return 0;

}