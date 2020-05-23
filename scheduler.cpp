#include "scheduler.h"

class core;

scheduler::scheduler()
{
}

scheduler::scheduler(vector<process*>* encerrados, vector<core*>* corePool, unsigned int schedul, unsigned int quantum){
    this->encerrados = encerrados;
    this->corePool = corePool;
    this->schedul = schedul;
    this->quantum = quantum;
}

void scheduler::scheduleProcess(process* p, core* c){
    p->state = RUNNING;
    c->p = p;
}

process* scheduler::descheduleProcess(core* c)
{
    process* p1 = c->p;
    c->p = nullptr;
    return p1;
}

process* scheduler::nextProcess()
{
    for (process* p : *ready_Queue) {
        if (p->state == READY) {
            return p;
        }
    }
}

void scheduler::removeProcess(int id)
{
    for (int i = 0; i < ready_Queue->size(); i++) {
        if (ready_Queue->at(i)->id == id) {
            ready_Queue->erase(ready_Queue->begin() + i);
        }
    }
}

void scheduler::insertionSort(vector<process*>& vetor)
{
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

void scheduler::run()
{
    while (true) {

        cout << "CPU:" << endl;
        cout << endl;
        for (core* c : *corePool) {

            if (c->p == nullptr) {
                process* p1 = nextProcess();
                scheduleProcess(p1, c);
            }


            c->p->remaningTime--;

            if (c->p->remaningTime == 0) {
                c->p->state = TERMINATED;
                cout << "CORE: " << c->id << " PROCESSO ENCERRADO: " << c->p->id << "\t";
                continue;

            }



            cout << "CORE: " << c->id << " P: " << c->p->id << " TR: " << c->p->remaningTime << "\t";
        }


        cout << endl << endl << endl;

        cout << "PROCESSOS:" << endl;
        cout << endl;

        for (process* p : *ready_Queue) {
            cout << "{[ID:" << p->id << "] , [TT:" << p->totalTime << "] , [TR:" << p->remaningTime << "] , [STATUS:" << p->state << "]}" << "\n";
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

void scheduler::setScheduleAlgorithm()
{
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

void scheduler::algorithmFIFO()
{

    cout << "\n" << "========================================================" << "\n";

    for (core* c : *corePool) {
        if (c->p->state == TERMINATED) {

            process* p1 = nextProcess();
            descheduleProcess(c);
            scheduleProcess(p1, c);


        }
    }

}

void scheduler::algorithmShortestFirst()
{
    cout << "\n" << "========================================================" << "\n";

    insertionSort(*ready_Queue);

    for (core* c : *corePool) {
        if (c->p->state == TERMINATED) {

            process* p1 = nextProcess();
            descheduleProcess(c);
            scheduleProcess(p1, c);

        }

    }
}

void scheduler::roundRobinAux(vector<process*>& vetor)
{
    process* p = vetor[0];
    for (int i = 0; i < ready_Queue->size() - 1; i++) {

        vetor[i] = vetor[i + 1];
    }
    vetor[ready_Queue->size() - 1] = p;
}

void scheduler::algorithmRoundRobin()
{

    if (coreAux.size() == 0) {
        for (int i = 0; i < corePool->size(); i++) {

            coreAux.emplace_back(corePool->at(i)->p->totalTime);

        }
    }

    cout << "\n" << "========================================================" << "\n";

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
