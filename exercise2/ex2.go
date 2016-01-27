// Go 1.2

package main

import (
    . "fmt"
    "runtime"
    "time"
)

var i int = 0



func someGoroutine1(c1 chan int) {
    for a := 0; a < 1000000; a++{
        for{
            select{
                case i_local := <- c1:
                    i_local++
                    c1 <- i_local
                    break
            }
        }
    } 

}

func someGoroutine2(c1 chan int) {
    for a := 0; a < 1000000; a++{
        for{
            select{
                case i_local := <-c1:
                    i_local--
                    c1 <- i_local
                    break
            }
        }
    } 
}

func main() {
    runtime.GOMAXPROCS(runtime.NumCPU())    // I guess this is a hint to what GOMAXPROCS does...
                                            // Try doing the exercise both with and without it!
    c1 := make(chan int)
    c1 <- i
    go someGoroutine1(c1) 
    go someGoroutine2(c1)                     // This spawns someGoroutine() as a goroutine

    // We have no way to wait for the completion of a goroutine (without additional syncronization of some sort)
    // We'll come back to using channels in Exercise 2. For now: Sleep.
    time.Sleep(100*time.Millisecond)
    Println(i)
}