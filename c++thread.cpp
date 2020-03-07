// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <vector>

using namespace std;

class core {
public:
    int id;
    int time;
    bool alive = true;
    core(int id, int time) {
        this->id = id;
        this->id = time;

        while (alive){
            time--;
            cout << "current Time: " << time << endl;
            this_thread::sleep_for(chrono::seconds(2));

            if (time == 0)
                alive = false;
        }
    }
};
    

class CPU {
public:
    vector<thread>* cores = new vector<thread>;
    vector<core> c1;

private:
    void init() {
        for (size_t i = 0; i < 3; i++)
        {
            c1.emplace_back(new core(i, i+10));
        }


        for (size_t i = 0; i < c1.size(); i++){
            cores->emplace_back(move(c1[i]));
        }
    }
};



int main(){

    CPU c = CPU();


    //std::thread second(&CPU::core, c);

    //second.join();

    return 0;
}