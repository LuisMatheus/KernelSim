#pragma once
#include"process.h"
#include"cpu.h"
#include"scheduler.h"
#include"memoryManager.h"


#include<vector>
#include<thread>

using namespace std;

class kernel {
private:

    unsigned int id;

    CPU* cpu;
    scheduler schd;
    memoryManager memMan;

public:

    vector<process*> pct;

    vector<process*> encerrados;

    kernel(CPU* cpu, unsigned  int sched, unsigned quantum, int memAlockAlgo, unsigned int totalInstalledMemory, unsigned int numberQuickList, unsigned int numberMemoryCalls);

    void run();

    void killProcess(int id);

    void createProcess();

    memoryBlock memoryAllocation(unsigned int SIZE);

    bool freeMemory(memoryBlock MEM);

};