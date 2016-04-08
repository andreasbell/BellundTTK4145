A working elevator
===============================

 - `io.c`, `io.h`, `channels.h`, `elev.c` and `elev.h` are the elevator drivers that provide an interface to controll the physical elevator.
 - `Elevator.cpp` and `Elevator.h` provide a elevator class that can independantly run an elevartor using a state machine. this can be done by first calling the `init()` finction and then running the `run()` function in a while loop.
 - `Network_Connection.cpp`, `UDP_Connection.cpp` and `UDP_Connection.h` are a networkinglibrarty for UDP that can send and recieve messeges over udp using unicast and multicast using the `.send()` and `.recieve()` functions. 
 - The `Manager.cpp` and `Manager.h` provide a manager class that can controll multiple elevators on a network using the udp and elevator classes.

The following links go to the original source:
 - The core C driver [is available here](https://github.com/TTK4145/Project/tree/master/driver).
 - The simulator [is available here](https://github.com/TTK4145/Project/tree/master/simulator).

