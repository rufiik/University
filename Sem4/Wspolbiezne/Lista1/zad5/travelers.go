package main

import (
    "fmt"
    "time"
    "math/rand"
    "sync"
    

)
const Nr_Of_Travelers =15

const Min_Steps =10
const Max_Steps =100

const Min_Delay=0.01
const Max_Delay=0.05

const Board_Width=15
const Board_Height=15

var Board [Board_Width][Board_Height]sync.Mutex

type Position_Type struct {
    X int
    Y int
}

func Move_Down(p *Position_Type){
    p.Y=(p.Y+1) % Board_Height
}

func Move_Up(p *Position_Type){
    p.Y=(p.Y +Board_Height-1) % Board_Height
}

func Move_Left(p *Position_Type){
    p.X=(p.X+Board_Width -1) % Board_Width
}

func Move_Right(p *Position_Type){
    p.X=(p.X+1 )% Board_Width
}
type Trace_Type struct {
    Time_Stamp time.Duration
    Id int
    Position Position_Type
    Symbol rune
}
type Trace_Array_Type []Trace_Type

type Traces_Sequence_Type struct {
    Last int
    Trace_Array Trace_Array_Type
}

func Print_Trace(Trace Trace_Type){
    fmt.Printf(
        " %.9f  %d  %d  %d  %c\n",
        Trace.Time_Stamp.Seconds(),          
        Trace.Id,                  
        Trace.Position.X,         
        Trace.Position.Y,          
        Trace.Symbol,             
    )
}

func Printer(reportChannel chan Traces_Sequence_Type) {
    for traces := range reportChannel { 
        for i := 0; i <len(traces.Trace_Array); i++ {
            Print_Trace(traces.Trace_Array[i])
        }
    }
}

type Traveler_Type struct {
    Id         int
    Symbol     rune
    Position   Position_Type
    Nr_of_Steps      int
    Delay      time.Duration
    Traces     Traces_Sequence_Type
    StartTime  time.Time
    ReportChan chan Traces_Sequence_Type
    Active     bool 
    Direction  func(*Position_Type)
}
var startTime= time.Now()
func (t *Traveler_Type)Store_Trace()  {
    t.Traces.Last++
    trace := Trace_Type{
        Time_Stamp: time.Since(startTime),
        Id:         t.Id,
        Position:   t.Position,
        Symbol:     t.Symbol,
    }
    t.Traces.Trace_Array = append(t.Traces.Trace_Array, trace)
}
func (t *Traveler_Type) Make_Step() {
    newPosition := t.Position
    t.Direction(&newPosition) 

    success := make(chan bool, 1)
    go func() {
        Board[newPosition.X][newPosition.Y].Lock()
        success <- true
    }()

    select {
    case <-success:
        
        // Jeśli udało się zablokować, zwolnij poprzednie pole i zaktualizuj pozycję
        Board[t.Position.X][t.Position.Y].Unlock()
        t.Position = newPosition
    case <-time.After(time.Duration(2*Max_Delay * float64(time.Second))):
        if t.Symbol >= 'A' && t.Symbol <= 'Z' {
            t.Symbol = rune(t.Symbol + 32) // Zamiana na małą literę
        }
        t.Store_Trace()
        t.Traces.Last++
        t.Active = false
        return
    }
}
    var InitMutex sync.Mutex
    func (t *Traveler_Type) Init(id int, symbol rune, reportChan chan Traces_Sequence_Type) {
        t.Id = id
        t.Symbol = symbol
        t.Position = Position_Type{X: id , Y: id } // Start na przekątnej
        Board[t.Position.X][t.Position.Y].Lock() 
        if id%2 == 0 {
            if rand.Intn(2) == 0 {
                t.Direction = Move_Up
            } else {
                t.Direction = Move_Down
            }
        } else {
            if rand.Intn(2) == 0 {
                t.Direction = Move_Left
            } else {
                t.Direction = Move_Right
            }
        }
        t.Active= true
        t.Nr_of_Steps = rand.Intn(Max_Steps-Min_Steps+1) + Min_Steps
        t.Traces = Traces_Sequence_Type{
            Last:        -1,
            Trace_Array: make([]Trace_Type, 0),
        }
        t.StartTime = startTime
        t.ReportChan = reportChan
    
        t.Traces.Trace_Array = append(t.Traces.Trace_Array, Trace_Type{
            Time_Stamp: 0,
            Id:         t.Id,
            Position:   t.Position,
            Symbol:     t.Symbol,
        })
    }


func (t *Traveler_Type) Start() {
    t.Active = true 
    for step := 0; step < t.Nr_of_Steps; step++ {
        if !t.Active {
            break 
        }
        delay := Min_Delay + (Max_Delay-Min_Delay)*float64(rand.Intn(100))/100
        time.Sleep(time.Duration(delay * float64(time.Second)))
        t.Make_Step()
        t.Store_Trace()
    }
    t.ReportChan <- t.Traces
}

func main() {
    rand.Seed(time.Now().UnixNano())
    fmt.Printf("-1  %d  %d  %d\n", Nr_Of_Travelers, Board_Width, Board_Height)
    reportChannel := make(chan Traces_Sequence_Type)
    var printerWg sync.WaitGroup
    printerWg.Add(1)

    go func() {
        defer printerWg.Done()
        Printer(reportChannel)
    }()
    

    travelers := make([]Traveler_Type, Nr_Of_Travelers)
    for i := 0; i < len(travelers); i++ {
        symbol := rune('A' + i ) 
        travelers[i].Init(i, symbol, reportChannel)
    }

    var wg sync.WaitGroup

    for i := 0; i < len(travelers); i++ {
        wg.Add(1)
        go func(t *Traveler_Type) {
            defer wg.Done()
            t.Start()
        }(&travelers[i])
    }
    wg.Wait()
    close(reportChannel)
    printerWg.Wait() 
}