#include "kernel.h"

kernel::kernel(CPU* cpu, unsigned int sched, unsigned quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls){
    id = 1;
    this->cpu = cpu;
    schd = scheduler(&encerrados, &cpu->corePool, sched, quantum);
    memMan = memoryManager(memAlockAlgo, totalInstalledMemory, numberQuickList, numberMemoryCalls);
}

void kernel::run() {
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

void kernel::killProcess(int id){
    for (int i = 0; i < pct.size(); i++) {
        if (pct.at(i)->id == id) {
            encerrados.emplace_back(pct.at(i));
            pct.erase(pct.begin() + i);
            schd.removeProcess(id);
        }
    }
}

void kernel::createProcess(){
    process* p = new process(id, rand() % 30 + 1, rand() % 1024 + 1);
    pct.emplace_back(p);
    schd.ready_Queue->emplace_back(p);
    id++;
}

memoryBlock kernel::memoryAllocation(unsigned int SIZE){
    return memoryBlock();
}

bool kernel::freeMemory(memoryBlock MEM){
    return false;
}
