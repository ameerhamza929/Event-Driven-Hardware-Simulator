#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <fstream>

using namespace std;

class Signal {
    string name;
    uint32_t value;
    vector<function<void(uint32_t)>> callback;

public:
    Signal(string nam, uint32_t val) : name(nam), value(val) {}

    string getname() {
        return name;
    }

    uint32_t getValue() {
        return value;
    }

    void setvalue(uint32_t newVal) {
        if (value != newVal) {
            value = newVal;

            for (function<void(uint32_t)>& cb : callback) {
                cb(newVal);
            }
        }
    }

    void addfunc(function<void(uint32_t)> cb) {
        callback.push_back(cb);
    }
};

