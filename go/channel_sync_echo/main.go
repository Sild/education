package main

import (
	"fmt"
	"sync"
	"time"
)

func reader(buffer <-chan int, wg *sync.WaitGroup) {
	defer wg.Done()
	for val := range buffer {
		time.Sleep(1 * time.Second)
		fmt.Printf("reader get: %d\n", val)
	}
	fmt.Println("reader done")
}

func writer(buffer chan<- int, count int, wg *sync.WaitGroup) {
	defer wg.Done()
	for i :=0; i < count; i++ {
		buffer <- i
		fmt.Printf("writer put: %d\n", i)
	}
	fmt.Println("writer done")
	close(buffer)
}

func main() {
	sChannel := make(chan int, 5)
	wg := &sync.WaitGroup{}

	wg.Add(2)
	go writer(sChannel, 12, wg)
	go reader(sChannel, wg)
	wg.Wait()

}