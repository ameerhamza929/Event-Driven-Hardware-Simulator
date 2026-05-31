#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <fstream>

using namespace std;

struct Event {
    Signal* signal;
    uint32_t newValue;
    uint64_t timestamp;    // in nanoseconds

    bool operator>(const Event& other) const {
        return timestamp > other.timestamp;
    }
};

class Scheduler{
    priority_queue<Event, vector<Event>, greater<Event>> que;
    uint64_t current_time;

    public:
        void setschedule (Signal *sig, uint32_t val, uint64_t timed){
            que.push({sig, val, timed});
        }

        void run(){
            while (!que.empty()){
                Event e = que.top();
                que.pop();
                current_time = e.timestamp;
                cout << "[" << current_time << " ns] "
                  << e.signal->getname()
                  << ": " << e.signal->getValue()
                  << " -> " << e.newValue << "\n";
                
                e.signal->setvalue(e.newValue);
            }

        }

        uint64_t getTime(){
            return current_time;
        }

};
