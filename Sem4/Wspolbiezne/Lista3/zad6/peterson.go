package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	NrOfProcesses = 2
	MinSteps      = 50
	MaxSteps      = 100
	MinDelay      = 10 * time.Millisecond
	MaxDelay      = 50 * time.Millisecond
)

type ProcessState int

const (
	LocalSection ProcessState = iota
	EntryProtocol
	CriticalSection
	ExitProtocol
)

var stateNames = []string{
	"Local_Section",
	"Entry_Protocol",
	"Critical_Section",
	"Exit_Protocol",
}

type Position struct {
	X int
	Y int
}

type Trace struct {
	Timestamp time.Duration
	ID        int
	Position  Position
	Symbol    rune
}

type Traces struct {
	List []Trace
}

var (
	flag    [NrOfProcesses]bool
	turn     int
	start    = time.Now()
	wg       sync.WaitGroup
)

func printTrace(trace Trace) {
	fmt.Printf("%.9f %d %d %d %c\n",
		trace.Timestamp.Seconds(),
		trace.ID,
		trace.Position.X,
		trace.Position.Y,
		trace.Symbol)
}

func printTraces(traces Traces) {
	for _, t := range traces.List {
		printTrace(t)
	}
}

func delayRandom(r *rand.Rand) {
	d := MinDelay + time.Duration(r.Float64()*float64(MaxDelay-MinDelay))
	time.Sleep(d)
}

func process(id int, symbol rune, seed int64) {
	defer wg.Done()
	r := rand.New(rand.NewSource(seed))
	traces := Traces{}

	position := Position{X: id, Y: int(LocalSection)}
	trace := func(state ProcessState) {
		t := time.Since(start)
		position.Y = int(state)
		traces.List = append(traces.List, Trace{
			Timestamp: t,
			ID:        id,
			Position:  position,
			Symbol:    symbol,
		})
	}

	trace(LocalSection)

	nrOfSteps := MinSteps + r.Intn(MaxSteps-MinSteps+1)
	nrOfSteps = nrOfSteps / 4 // For testing

	for step := 0; step < nrOfSteps; step++ {
		delayRandom(r)

		trace(EntryProtocol)
		me := id
		other := 1 - id
		flag[me] = true
		turn = other
		for flag[other] && turn == other {
			// busy wait
		}
		trace(CriticalSection)
		delayRandom(r)

		trace(ExitProtocol)

		flag[me] = false
		trace(LocalSection)
	}
	printTraces(traces)
}

func main() {
	wg.Add(NrOfProcesses)
	symbol := 'A'

	for i := 0; i < NrOfProcesses; i++ {
		go process(i, symbol, time.Now().UnixNano()+int64(i*100))
		symbol++
	}

	wg.Wait()

	fmt.Printf("-1 %d %d %d ", NrOfProcesses, NrOfProcesses, len(stateNames))
	for _, name := range stateNames {
		fmt.Print(name + ";")
	}
	fmt.Println()
}
