package main

import (
    "runtime"
    "fmt"
)

func main() {
	PrintMemUsage()

    	m := make(map[int]int)

	for i := 1; i < 50000000; i++ {
		m[i] = 1
	}
    	// Clear our memory and print usage, unless the GC has run 'Alloc' will remain the same
    	PrintMemUsage()

    	// Force GC to clear up, should see a memory drop
    	runtime.GC()
    	PrintMemUsage()
	
}

// PrintMemUsage outputs the current, total and OS memory being used. As well as the number 
// of garage collection cycles completed.
func PrintMemUsage() {
        var m runtime.MemStats
        runtime.ReadMemStats(&m)
        // For info on each, see: https://golang.org/pkg/runtime/#MemStats
        fmt.Printf("Alloc = %v MiB", bToMb(m.Alloc))
        fmt.Printf("\tTotalAlloc = %v MiB", bToMb(m.TotalAlloc))
        fmt.Printf("\tSys = %v MiB", bToMb(m.Sys))
        fmt.Printf("\tNumGC = %v\n", m.NumGC)
}

func bToMb(b uint64) uint64 {
    return b / 1024 / 1024
}

