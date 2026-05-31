#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <fstream>

using namespace std;

class Logger {
private:
    ofstream logFile;

public:
    Logger(string filename){
        logFile.open(filename);
    }
    void attach(Signal* sig, Scheduler* sched){
        sig->addfunc([sig, sched, this](uint32_t val) {
        string entry = "[" + to_string(sched->getTime()) +
                            " ns] " + sig->getname() +
                            " = " + to_string(val) + "\n";
        logFile << entry;
        std::cout << entry;
       });
    }
    ~Logger(){
        logFile.close();
    }
};
