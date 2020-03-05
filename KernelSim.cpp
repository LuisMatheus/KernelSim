#include <iostream>
#include <list>

struct process{
    unsigned int id;
    unsigned int totalTime;
    // 0: READY , 1: RUNNING , -1: TERMINATED
    int state;
    unsigned int remaningTime;
};

class kernel
{
public:
    
    void run() {}
    void createProcess() {};
    void killProcess() {};
    void runProcess() {};
    //cpu: classe que representa a CPU e seus cores de processamento.
    void CPU() {};

private:
    std::list<process> pct;
};

class scheduler {
public:


private:


};

class simulator {
public:

    void run() {};
    void createRandomProcess() {};
    void batchProcessInit() {};


private:
    kernel ker = kernel();

    // 1 ~ 64
    unsigned int coreNumber = 0;

    // 2 ~ 20
    unsigned int quantum = 0;
};


int main(){
    std::cout << "Hello World!\n";
}