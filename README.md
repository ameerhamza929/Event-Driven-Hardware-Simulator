# Event-Driven Hardware Simulator

A C++ based event-driven hardware simulator that models digital logic circuits with precise timing and signal propagation. This simulator uses an event queue to efficiently handle discrete time steps and component state changes.

## Overview

This project implements a discrete-event simulation framework for digital hardware circuits. It accurately models the behavior of digital logic gates, flip-flops, and signal propagation with configurable delays. The simulator is useful for understanding digital circuit behavior, testing logic designs, and learning about hardware simulation principles.

## Key Features

- **Event-Driven Architecture**: Efficient simulation using a priority queue-based event scheduler
- **Signal Propagation**: Accurate modeling of signal changes and callbacks
- **Digital Logic Gates**: Implementations of AND, OR, and NOT gates
- **Sequential Logic**: D Flip-Flop with clock-based edge triggering
- **Configurable Delays**: Each component has adjustable propagation delays
- **Logging System**: Built-in signal monitoring and logging to file
- **Timestamp Precision**: Nanosecond-level simulation time tracking

## Architecture

### Core Components

#### 1. **Signal** (`Signal.cpp`)
Represents a digital signal in the circuit.
- **Value**: 32-bit unsigned integer representing the signal state
- **Callbacks**: Observer pattern implementation for change notifications
- **Methods**:
  - `getname()`: Returns the signal identifier
  - `getValue()`: Gets the current signal value
  - `setvalue(newVal)`: Updates signal value and triggers callbacks
  - `addfunc(callback)`: Registers a callback function for signal changes

#### 2. **Scheduler** (`Scheduler.cpp`)
Manages event scheduling and execution.
- **Event Queue**: Priority queue ordered by timestamp
- **Time Management**: Tracks current simulation time in nanoseconds
- **Methods**:
  - `setschedule()`: Schedules a signal change at a specific time
  - `run()`: Executes all scheduled events in chronological order
  - `getTime()`: Returns the current simulation time

#### 3. **Components** (`Components.cpp`)
Base class and derived gate implementations.

**Base Class - Component**:
- Abstract base for all digital components
- Properties: name, delay, scheduler reference
- Virtual `evaluate()` method for subclass implementation

**Digital Gates**:

- **ANDGate**: Implements AND operation
  - Inputs: inputA, inputB
  - Output: result of (inputA & inputB)
  - Default delay: 2 ns
  - Triggers on any input change

- **ORGate**: Implements OR operation
  - Inputs: inputA, inputB
  - Output: result of (inputA | inputB)
  - Default delay: 2 ns
  - Triggers on any input change

- **NOTGate**: Implements NOT operation
  - Input: single input signal
  - Output: 1-bit inverted result
  - Default delay: 1 ns
  - Triggers on input change

- **DFlipFlop**: Positive-edge-triggered D Flip-Flop
  - Data Input (D): data to be captured
  - Clock Input (CLK): synchronization signal
  - Output (Q): captured data
  - Default delay: 3 ns
  - Captures data on rising edge (CLK = 1)

#### 4. **Logger** (`Logger.cpp`)
Monitors signal changes and logs them.
- **File Output**: Writes simulation events to a log file
- **Console Output**: Displays events in real-time
- **Timestamp Inclusion**: Each log entry includes the simulation time
- **Methods**:
  - `attach()`: Attaches logger to a signal for monitoring
  - Logs format: `[timestamp ns] signal_name = value`

### Simulation Workflow

1. **Initialization**: Create signals, components, and scheduler
2. **Stimulus**: Schedule input signal changes using `setschedule()`
3. **Execution**: Call `scheduler.run()` to process all events
4. **Output**: View results in console and `simulation.log` file

## Usage Example

The included `Testbench.cpp` demonstrates a complete simulation:

```cpp
// Create simulator and logger
Scheduler sim;
Logger logger("simulation.log");

// Create signals
Signal A("A", 0);
Signal B("B", 0);
Signal CLK("CLK", 0);
Signal AND_OUT("AND_OUT", 0);
Signal NOT_OUT("NOT_OUT", 0);
Signal Q("Q", 0);

// Attach logger to monitor all signals
logger.attach(&A, &sim);
logger.attach(&B, &sim);
logger.attach(&CLK, &sim);
logger.attach(&AND_OUT, &sim);
logger.attach(&NOT_OUT, &sim);
logger.attach(&Q, &sim);

// Build circuit: A,B -> AND -> NOT -> DFF with CLK -> Q
ANDGate and1("AND1", &A, &B, &AND_OUT, &sim, 2);
NOTGate not1("NOT1", &AND_OUT, &NOT_OUT, &sim, 1);
DFlipFlop dff1("DFF1", &CLK, &NOT_OUT, &Q, &sim, 3);

// Schedule stimulus
sim.setschedule(&A, 1, 10);     // A = 1 at 10 ns
sim.setschedule(&B, 1, 15);     // B = 1 at 15 ns
sim.setschedule(&CLK, 1, 20);   // CLK rising edge at 20 ns
sim.setschedule(&CLK, 0, 25);   // CLK falling edge at 25 ns
sim.setschedule(&B, 0, 30);     // B = 0 at 30 ns
sim.setschedule(&CLK, 1, 40);   // CLK rising edge at 40 ns

// Run simulation
sim.run();

// Display final time
cout << "Final Time = " << sim.getTime() << " ns\n";
```

### Circuit Diagram
```
A ----\
       AND ---- AND_OUT ---- NOT ---- NOT_OUT ---- DFF ---- Q
B ----/                                    ^
                                           |
                                          CLK
```

## Compilation

To compile the project:

```bash
g++ -std=c++11 -o simulator Testbench.cpp
```

Or using a Makefile (if available):
```bash
make
```

## Output

The simulator produces two outputs:

1. **Console Output**: Real-time display of signal changes
   ```
   [10 ns] A = 0 -> 1
   [12 ns] AND_OUT = 0 -> 0
   ...
   ```

2. **Log File** (`simulation.log`): Persistent record of all signal changes
   ```
   [10 ns] A = 1
   [15 ns] B = 1
   [17 ns] AND_OUT = 1
   [18 ns] NOT_OUT = 0
   ...
   ```

## Design Patterns Used

- **Observer Pattern**: Signal callbacks notify components of state changes
- **Strategy Pattern**: Component virtual `evaluate()` methods
- **Priority Queue**: Efficient event scheduling by timestamp

## Time Complexity

- **Signal Change**: O(n) where n is the number of registered callbacks
- **Schedule Event**: O(log m) where m is the number of scheduled events
- **Simulation Run**: O(m log m) for processing all m events

## Project Structure

```
Event-Driven-Hardware-Simulator/
├── Signal.cpp          # Signal class implementation
├── Scheduler.cpp       # Event scheduler implementation
├── Components.cpp      # Gate and flip-flop implementations
├── Logger.cpp          # Logging system
├── Testbench.cpp       # Main simulation entry point
└── README.md           # This file
```

## Future Enhancements

- Add header files (.h) for better code organization
- Implement additional gate types (XOR, NAND, NOR, XNOR)
- Add asynchronous reset to flip-flops
- Implement memory and register elements
- Add waveform viewers
- Create reusable circuit modules
- Add constraint and timing analysis
- Support for bus signals and multi-bit logic

## Technical Details

### Event Processing
Events are processed in strict chronological order using a C++ `priority_queue`. The scheduler ensures:
- Causality: Events are executed in timestamp order
- Determinism: Identical stimuli produce identical results
- Efficiency: O(log n) insertion for each event

### Signal Change Detection
The Signal class implements change detection:
- Only triggers callbacks when value actually changes (no redundant updates)
- Prevents infinite loops from circular signal dependencies
- Enables efficient delta-cycle simulation

### Clock Synchronization
The D Flip-Flop captures data only on rising edges (CLK = 1):
- Ensures proper sequential logic behavior
- Prevents data metastability issues
- Synchronizes component updates

## Author

**Ameer Hamza**  
Repository: [Event-Driven-Hardware-Simulator](https://github.com/ameerhamza929/Event-Driven-Hardware-Simulator)

## License

This project is provided as-is for educational and research purposes.

---

**Last Updated**: May 31, 2026
