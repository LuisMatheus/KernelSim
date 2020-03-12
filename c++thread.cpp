#include <iostream>
#include <thread>
#include <vector>

using namespace std;

class process {
public:
    int id;
    int timeTotal;
    int timeRemaning;
   process(int id, int time) {
        this->id = id;
        this->timeTotal = time;
        this->timeRemaning = this->timeTotal;
    }
};

class core {
public:
    int id;
    process* p = NULL;
    core(int id) {
        this->id = id;
    }

    void processTask() {
        this->p->timeRemaning--;
        cout << "CORE: " << id << "Time Remaning: " << p->timeRemaning << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
    
};
    

class CPU {
public:
    vector<core> c1;
    int coreNumber;

    CPU(int coreNumber) {
        this->coreNumber = coreNumber;
        for (size_t i = 0; i < this->coreNumber; i++)
        {
            c1.emplace_back(i);
        }
    }

    void processTask() {

        for (core c : c1) {
            c.processTask();
        }
              
    }
private:
   
};



int main(){

    CPU c = CPU(2);
    vector<process*> cpt;
    cpt.emplace_back(new process(0, 15));
    cpt.emplace_back(new process(1, 12));
    c.c1.at(0).p = cpt.at(0);
    c.c1.at(1).p = cpt.at(1);
    c.processTask();
    //thread cpuT(&c);
    //thread th1(&CPU::processTask,&c);
    //th1.join();

    return 0;
}