A working elevator
===============================

 - `io.c`, `io.h`, `channels.h`, `elev.c` and `elev.h` are the elevator drivers that provide an interface to controll the physical elevator.
 - `Elevator.cpp` and `Elevator.h` provide a elevator class that can independantly run an elevartor using a state machine. this can be done by first calling the `init()` finction and then running the `run()` function in a while loop.

The following links go to the original source:
 - The core C driver [is available here](https://github.com/TTK4145/Project/tree/master/driver).
 - The simulator [is available here](https://github.com/TTK4145/Project/tree/master/simulator).

