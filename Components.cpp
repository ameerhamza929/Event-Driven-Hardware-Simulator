#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <queue>
#include <fstream>

using namespace std;

class Component{
    protected:
        string name;
        uint64_t delay;
        Scheduler *schedule;

    public:
        Component(Scheduler *sc , string nd, uint64_t del): schedule(sc), name(nd),delay(del){}
        virtual void evaluate() = 0;    
        string getName(){
            return name;
        }
        virtual ~Component() {} 
};

class ANDGate : public Component {
private:
    Signal* inputA;
    Signal* inputB;
    Signal* output;

public:
    ANDGate(string name, Signal* a, Signal* b,
            Signal* out, Scheduler* sched, uint64_t delay = 2): Component(sched,name, delay), inputA(a), inputB(b), output(out) {

        // Register evaluate() to fire whenever inputA or inputB changes
        auto trigger = [this](uint32_t) { this->evaluate(); };
        inputA->addfunc(trigger);
        inputB->addfunc(trigger);
    }
    void evaluate() override{
        uint32_t result = inputA->getValue() & inputB->getValue();
        schedule->setschedule(output, result, schedule->getTime() + delay);
    }
};

class ORGate : public Component {
private:
    Signal* inputA;
    Signal* inputB;
    Signal* output;

public:
    ORGate(std::string name, Signal* a, Signal* b,
           Signal* out, Scheduler* sched, uint64_t delay = 2): Component(sched,name, delay), inputA(a), inputB(b), output(out) {

        auto trigger = [this](uint32_t) { this->evaluate(); };
        inputA->addfunc(trigger);
        inputB->addfunc(trigger);
    }
    void evaluate() override{
        uint32_t result = inputA->getValue() | inputB->getValue();
        schedule->setschedule(output, result, schedule->getTime() + delay);
    }
};

class NOTGate : public Component {
private:
    Signal* input;
    Signal* output;

public:
    NOTGate(string name, Signal* in,
            Signal* out, Scheduler* sched, uint64_t delay = 1) : Component(sched,name, delay), input(in), output(out) {

            auto trigger = [this](uint32_t) { this->evaluate(); };
            input->addfunc(trigger);
    }
    void evaluate() override{
        uint32_t result = (~input->getValue()) & 0x1;  // mask to 1 bit
        schedule->setschedule(output, result, schedule->getTime() + delay);
    }
};


class DFlipFlop : public Component {
private:
    Signal* clk;
    Signal* d;       // data input
    Signal* q;       // output

    uint32_t lastClk = 0;

public:
    DFlipFlop(std::string name, Signal* clk, Signal* d,
              Signal* q, Scheduler* sched, uint64_t delay = 3) :Component(sched,name, delay), clk(clk), d(d), q(q) {

        // Only capture on RISING EDGE of clock
        clk->addfunc([this](uint32_t val) {
            if (val == 1) this->evaluate();   
        });
    }
    void evaluate() override{
         uint32_t captured = d->getValue();
         schedule->setschedule(q, captured, schedule->getTime() + delay);
    }
};
