package main

import "fmt"

func reader(buffer chan int) {
	var val int
	for {
		val = <-buffer
		fmt.Printf("reader get: %d\n", val)
		if val == 999 {
			fmt.Println("reader done")
			break
		}
	}
}

func writer(buffer chan int, count int) {
	for i :=0; i < count; i++ {
		buffer <- i
		fmt.Printf("writer put: %d\n", i)
	}
	fmt.Println("Writer done")
}

func main() {
	sChannel := make(chan int)

	go writer(sChannel, 12)
	go reader(sChannel)
	fmt.Scanln()

}