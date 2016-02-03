Exercise 5 : A working elevator
===============================

 - `io.c` and `io.h` define the core functions needed to read from and write to the libComedi device
 - `channels.h` gives readable names to the subdevices/channels used with the elevator
 - `elev.c` and `elev.h` show one way of turning `io` and `channels` into a higher level abstraction
 - `main.c` is an incomplete test program to verify the functionality of the `elev` API.

The following links go to the original source:
 - The core C driver [is available here](https://github.com/TTK4145/Project/tree/master/driver).
 - The simulator [is available here](https://github.com/TTK4145/Project/tree/master/simulator).





