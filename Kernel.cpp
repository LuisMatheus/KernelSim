#include "Kernel.h"

kernel::kernel(CPU* cpu, unsigned int sched, unsigned quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls){
    this->cpu = cpu;
    schd = scheduler(&encerrados, &cpu->corePool, sched, quantum);
    memMan = memoryManager(memAlockAlgo, totalInstalledMemory, numberQuickList, numberMemoryCalls);
}

void kernel::run(){
    thread schThread(&scheduler::run, schd);
    this_thread::sleep_for(chrono::milliseconds(500));
    while (true) {

        cout << endl <<"=======================================================" << endl;
        
        cout << "CORES: " << endl;

        for (core* c : cpu->corePool) {
            cout << "CORE ID: "<<c->id << " , PROCESSO ID:" << c->p->id << " , TEMPO RESTANTE: "<< c->p->remaningTime << "\t\t";
        }

        cout << endl << endl << "LIVRES:" << endl;

        memoryBlock* aux = memMan.freeBlocksList;
        while (aux!= nullptr) {
            cout <<"{" << aux->blockAdress <<" , " <<aux->totalBlockSize << " }" << "\t";

            aux = aux->nextFreeBlock;
        }

        /*
        cout << endl <<endl << "PROCESSOS:" << endl;

        for (process* p : *schd.ready_Queue) {
            cout << "{[ID:" << p->id << "] , [TT:" << p->totalTime << "] , [TR:" << p->remaningTime << "] , [STATUS:" << p->state << "] , [MEMORY:" << p->totalMemoryUsed << "]}" << "      ";
        }
        
        cout << endl;
        */
        cout << endl <<"MEMORIA:" << endl;
        if (memMan.allocationAlgorithim == 3) {
            for (vector<memoryBlock*> q : memMan.quickFitBlocks) {
                for (memoryBlock* m : q) {
                    cout << "[" << m->blockAdress << "," << m->totalBlockSize << "]" << " ";
                }
                cout << endl << "-------------" << endl;
            }
        }
        
        cout << "TOTAL: " << memMan.totalMemory << "\t" << "OCUPADO: " << memMan.occupiedMemory + memMan.memoryOverhead << "\t" << "DISPONIVEL: " << memMan.getAvailableMemory() << endl;
        /*
        for (memoryBlock* m : memMan.memory) {
            cout << "[" << m->blockAdress << "," << m->totalBlockSize << "," << m->occupiedSize << "]" << " ";
        }
        */
        
        cout << endl << endl;

        cout << "ENCERRADOS:" << endl;

        for (process* p : encerrados) {
            cout << "[ID: " << p->id << "," << p->state << "]  ";;
        }
        
        cout << endl;
        cout << endl;
        cout << endl;



        for (process* p : pct) {

            if (p->state == TERMINATED) {
                killProcess(p->id);
            }

            if (rand() % 10 + 1 <= 2) {
                memoryBlock* m = memoryAllocation(pow(2, rand() % 8 + 1));
                if (m != nullptr) {
                    p->generateRandomDynamicMemoryCall(m);
                }
                else {
                    p->state = ABORTED;
                    killProcess(p->id);
                    cout << " DYNAMIC CALL FAILED - OUT OF MEMORY - PROCESS: " << p->id << " || ";
                }

            }
        }

        this_thread::sleep_for(chrono::seconds(1));
    }
    schThread.join();
}

void kernel::killProcess(int id) {
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

void kernel::createProcess(){
    process* p = new process(id++, rand() % 20 + 1, pow(2, rand() % 10 + 1));
    p->generateRandomStaticMemoryCall(memoryAllocation(p->totalMemoryUsed));
    if (!p->memoryPointer.empty()) {
        pct.emplace_back(p);
        schd.ready_Queue->emplace_back(p);
    }
    else {
        p->state = ABORTED;
        cout << "  STATIC CALL FAILED - SYSTEM OUT OF MEMORY:" << p->id << " || ";
        encerrados.emplace_back(p);
    }
}

memoryBlock* kernel::memoryAllocation(unsigned int SIZE){
    if (memMan.checkFreeMemory(SIZE)) {
        return memMan.malloc(SIZE);
    }
    return nullptr;
}

void kernel::freeMemory(process* p){
    for (memoryBlock* m : p->memoryPointer) {
        memMan.free(m);
    }
    p->memoryPointer.clear();
}