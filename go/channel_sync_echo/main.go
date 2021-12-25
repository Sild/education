package main

import "fmt"

func reader(buffer <-chan int) {
	for val := range buffer {
		val = <-buffer
		fmt.Printf("reader get: %d\n", val)
	}
	fmt.Println("reader done")
}

func writer(buffer chan<- int, count int) {
	for i :=0; i < count; i++ {
		buffer <- i
		fmt.Printf("writer put: %d\n", i)
	}
	fmt.Println("writer done")
	close(buffer)
}

func main() {
	sChannel := make(chan int)

	go writer(sChannel, 12)
	go reader(sChannel)
	fmt.Scanln()

}