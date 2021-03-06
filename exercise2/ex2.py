
# Python 3.3.3 and 2.7.6
# python helloworld_python.py

from threading import Thread
from threading import Lock
i = 0

lock = Lock()

def someThreadFunction1():
    global i
    for a in range(1000000):
        with lock:
            i += 1

def someThreadFunction2():
    global i
    for a in range(1000001):
        with lock:
            i-= 1


def main():
    someThread1 = Thread(target = someThreadFunction1, args = (),)
    someThread2 = Thread(target = someThreadFunction2, args = (),)
    someThread1.start()
    someThread2.start()

    
    someThread1.join()
    someThread2.join()

    print(i);


main()