#pragma once

#include <iostream>

#include "Scheduler.h"
#include "MemoryManager.h"
#include "CPU.h"

using namespace std;

class kernel {
private:

    CPU* cpu;
    scheduler schd;
    memoryManager memMan;

    unsigned int id = 1;

public:

    vector<process*> pct;

    vector<process*> encerrados;

    kernel(CPU* cpu, unsigned  int sched, unsigned quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls);

    void run();

    void killProcess(int id);

    void createProcess();

    memoryBlock* memoryAllocation(unsigned int SIZE);

    void freeMemory(process* p);

};