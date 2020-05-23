#pragma once
#include"scheduler.h"
#include"process.h"
#include"cpu.h"
#include"memoryManager.h"


#include<vector>
#include<thread>

using namespace std;

class scheduler;

class process;

class CPU;

class kernel {
private:

    unsigned int idCounter;

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