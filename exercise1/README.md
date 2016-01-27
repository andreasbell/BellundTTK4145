# [Exercise 1 : Hello World](https://github.com/TTK4145/Exercises/tree/master/Ex01-Intro)

### Task 1
- We thought about elevators

### Task 2
- We implemented git

### Task 3

- Concurrent programming is when multiple processes lifetime overlap, as in thread programming, while in parallel processes the process actually runs at the same time.

- Multicore computers have become increasingly popular since they enable paralell programming.

- A thread is a "lightweight process", a process can consist of multiple threads. A green thred is a thread not run by the OS itself but emulated by software scheduler. Coroutines is a subroutine for nonpreemptive multitasking.

- ```pthread_create()``` (C) creates real threads, ```threading.Thread() ``` (Python) creates green threads, and ```go``` (Go) creates green threads in its own scheduler.

- Pythons Global Interpreter Lock stops a python program to execute more than one thread at a time.

- ????

- ````func GOMAXPROCS(n int) int``` limits the number of operating system threads that can execute Go code simultaneously.

### Task 4
```
global shared int i = 0

main:
    spawn thread_1
    spawn thread_2
    join all threads (wait for them to finish)
    print i

thread_1:
    do 1_000_000 times:
        i++
thread_2:
    do 1_000_000 times:
        i--
```

Runing this code does not always end with the ``` i = 0 + 1,000,000 - 1,000,000 = 0 ``` since when the threads execute the code they first fetch the value of i than they increment/decrement the value, and then eventually save the new value in i. If both threads fetch the value before the other thread has time to save the new incremented value, the value that last thread than saves will not reflect the changes tat the other thread made. 



