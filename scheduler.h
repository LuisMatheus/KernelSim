#pragma once
#include"process.h"
#include"core.h"

#include<thread>
#include<iostream>
#include<vector>

using namespace std;

class scheduler {
public:

    vector<process*>* ready_Queue = new vector<process*>();
    vector<process*>* encerrados;
    vector<core*>* corePool;

    vector<int> coreAux;

    unsigned int schedul;
    unsigned int quantum;

    scheduler();

    scheduler(vector<process*>* encerrados, vector<core*>* corePool, unsigned int schedul, unsigned int quantum);

    void scheduleProcess(process* p, core* c);

    process* descheduleProcess(core* c);

    process* nextProcess();

    void removeProcess(int id);

    void insertionSort(vector<process*>& vetor);

    void run();

    void setScheduleAlgorithm();

    void algorithmFIFO(); 

    void algorithmShortestFirst();

    void roundRobinAux(vector<process*>& vetor);

    void algorithmRoundRobin();

};