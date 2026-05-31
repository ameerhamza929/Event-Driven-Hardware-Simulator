int main() {

    Scheduler sim;
    Logger logger("simulation.log");

    // Input signals
    Signal A("A", 0);
    Signal B("B", 0);
    Signal CLK("CLK", 0);

    // Internal signals
    Signal AND_OUT("AND_OUT", 0);
    Signal NOT_OUT("NOT_OUT", 0);
    Signal Q("Q", 0);

    // Attach logger to all signals
    logger.attach(&A, &sim);
    logger.attach(&B, &sim);
    logger.attach(&CLK, &sim);
    logger.attach(&AND_OUT, &sim);
    logger.attach(&NOT_OUT, &sim);
    logger.attach(&Q, &sim);

    // Circuit:
    //
    // A ----\
    //        AND ---- AND_OUT ---- NOT ---- NOT_OUT ---- DFF ---- Q
    // B ----/                                    ^
    //                                             |
    //                                            CLK

    ANDGate and1("AND1",
                 &A,
                 &B,
                 &AND_OUT,
                 &sim,
                 2);

    NOTGate not1("NOT1",
                 &AND_OUT,
                 &NOT_OUT,
                 &sim,
                 1);

    DFlipFlop dff1("DFF1",
                   &CLK,
                   &NOT_OUT,
                   &Q,
                   &sim,
                   3);

    // Stimulus

    // A = 1 at 10ns
    sim.setschedule(&A, 1, 10);

    // B = 1 at 15ns
    sim.setschedule(&B, 1, 15);

    // Rising edge of clock at 20ns
    sim.setschedule(&CLK, 1, 20);

    // Falling edge at 25ns
    sim.setschedule(&CLK, 0, 25);

    // B = 0 at 30ns
    sim.setschedule(&B, 0, 30);

    // Rising edge at 40ns
    sim.setschedule(&CLK, 1, 40);

    cout << "\n===== Starting Simulation =====\n\n";

    sim.run();

    cout << "\n===== Simulation Complete =====\n";
    cout << "Final Time = "
         << sim.getTime()
         << " ns\n";

    return 0;
}
