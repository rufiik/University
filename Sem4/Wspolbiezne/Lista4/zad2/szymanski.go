package main

import (
    "fmt"
    "math/rand"
    "sync"
    "time"
)

const (
    NrOfProcesses = 20
    MinSteps      = 50
    MaxSteps      = 100
    MinDelay      = 10 * time.Millisecond
    MaxDelay      = 50 * time.Millisecond
)

type ProcessState int

const (
    LocalSection ProcessState = iota
    EntryProtocol1
    EntryProtocol2
    EntryProtocol3
    EntryProtocol4
    CriticalSection
    ExitProtocol
)

var stateNames = []string{
    "Local_Section",
    "Entry_Protocol_1",
    "Entry_Protocol_2",
    "Entry_Protocol_3",
    "Entry_Protocol_4",
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
    flag  [NrOfProcesses]int
    start = time.Now()
    wg    sync.WaitGroup
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

        // ENTRY_PROTOCOL_1: flag[id] = 1
        flag[id] = 1
        trace(EntryProtocol1)

        // await(all flag[j] in {0,1,2})
        for {
            ok := true
            for j := 0; j < NrOfProcesses; j++ {
                if flag[j] != 0 && flag[j] != 1 && flag[j] != 2 {
                    ok = false
                    break
                }
            }
            if ok {
                break
            }
            time.Sleep(1 * time.Millisecond)
        }

        // ENTRY_PROTOCOL_3: flag[id] = 3
        flag[id] = 3
        trace(EntryProtocol3)

        // if any flag[j] == 1
        any1 := false
        for j := 0; j < NrOfProcesses; j++ {
            if j != id && flag[j] == 1 {
                any1 = true
                break
            }
        }
        if any1 {
            // flag[id] = 2
            flag[id] = 2
            trace(EntryProtocol2)

            // await(any flag[j] == 4)
            for {
                found := false
                for j := 0; j < NrOfProcesses; j++ {
                    if flag[j] == 4 {
                        found = true
                        break
                    }
                }
                if found {
                    break
                }
                time.Sleep(1 * time.Millisecond)
            }
        }

        // flag[id] = 4
        flag[id] = 4
        trace(EntryProtocol4)

        // await(all flag[j] in {0,1} for j < id)
        for {
            ok := true
            for j := 0; j < id; j++ {
                if flag[j] != 0 && flag[j] != 1 {
                    ok = false
                    break
                }
            }
            if ok {
                break
            }
            time.Sleep(1 * time.Millisecond)
        }

        trace(CriticalSection)
        delayRandom(r)

        // EXIT_PROTOCOL: await(all flag[j] in {0,1,4} for j > id)
        for {
            ok := true
            for j := id + 1; j < NrOfProcesses; j++ {
                if flag[j] != 0 && flag[j] != 1 && flag[j] != 4 {
                    ok = false
                    break
                }
            }
            if ok {
                break
            }
            time.Sleep(1 * time.Millisecond)
        }

        trace(ExitProtocol)
        flag[id] = 0
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