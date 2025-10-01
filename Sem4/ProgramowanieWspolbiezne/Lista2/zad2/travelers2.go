package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

const (
	NrOfTravelers    = 15
	NrOfWildTenants  = 10
	NrOfAll          = NrOfTravelers + NrOfWildTenants
	MinSteps         = 10
	MaxSteps         = 100
	MinDelay         = 0.01
	MaxDelay         = 0.05
	BoardWidth       = 15
	BoardHeight      = 15
	MoveTimeout      = 0.15
	WildTenantMinDur = 0.5
)

type Position struct {
	X, Y int
}

type Trace struct {
	Timestamp time.Duration
	ID        int
	Position  Position
	Symbol    rune
}

type TraceSequence struct {
	Traces []Trace
}

type FieldRequestType int

const (
	CheckOccupant FieldRequestType = iota
	SetOccupant
	MoveOutRequest
	Leave
)

type FieldRequest struct {
	Type       FieldRequestType
	RequesterID int
	NewOccupant interface{}
	Response   chan interface{}
}

type Traveler struct {
	ID       int
	Symbol   rune
	Position Position
	Steps    int
	Seed     int64
	Dead     bool
}

type WildTenant struct {
	ID           int
	Symbol       rune
	Position     Position
	disappeared  bool
	moving       bool
	TimeBegin    time.Duration
	TimeEnd      time.Duration
	StopChan     chan struct{}
	MoveOutChan  chan struct{}
}

var (
	wg        sync.WaitGroup
	startTime = time.Now()
)

func fieldServer(x, y int, requestChan chan FieldRequest) {
	var occupant interface{}
	for req := range requestChan {
		switch req.Type {
		case CheckOccupant:
			req.Response <- occupant

		case SetOccupant:
			if occupant == nil {
				occupant = req.NewOccupant
				req.Response <- true
			} else {
				req.Response <- false
			}

		case MoveOutRequest:
			if wt, ok := occupant.(*WildTenant); ok && !wt.disappeared && !wt.moving {
				select {
				case wt.MoveOutChan <- struct{}{}:
					req.Response <- true
				default:
					req.Response <- false
				}
			} else {
				req.Response <- false
			}

		case Leave:
			occupant = nil
			req.Response <- true
		}
	}
}


func initBoard() [][]chan FieldRequest {
    board := make([][]chan FieldRequest, BoardWidth)
    for i := range board {
        board[i] = make([]chan FieldRequest, BoardHeight)
        for j := range board[i] {
            board[i][j] = make(chan FieldRequest)
            go fieldServer(i, j, board[i][j])
        }
    }
    return board
}


func moveDirection(pos Position, dir int) Position {
	newPos := pos
	switch dir {
	case 0: // Up
		newPos.Y = (pos.Y - 1 + BoardHeight) % BoardHeight
	case 1: // Down
		newPos.Y = (pos.Y + 1) % BoardHeight
	case 2: // Left
		newPos.X = (pos.X - 1 + BoardWidth) % BoardWidth
	case 3: // Right
		newPos.X = (pos.X + 1) % BoardWidth
	}
	return newPos
}

func (t *Traveler) tryMove(newPos Position, board [][]chan FieldRequest) bool {
    // Sprawdzenie, czy pole jest wolne
    resp := make(chan interface{})
    board[newPos.X][newPos.Y] <- FieldRequest{
        Type:     CheckOccupant,
        Response: resp,
    }
    occupant := <-resp

    if occupant == nil {
        // Próba zajęcia pola
        setResp := make(chan interface{})
        board[newPos.X][newPos.Y] <- FieldRequest{
            Type:        SetOccupant,
            NewOccupant: t,
            Response:    setResp,
        }
        if success := (<-setResp).(bool); success {
            // Zwolnienie starego pola
            leaveResp := make(chan interface{})
            board[t.Position.X][t.Position.Y] <- FieldRequest{
                Type:     Leave,
                Response: leaveResp,
            }
            <-leaveResp

            t.Position = newPos
            return true
        }
        return false
    }

    if wt, ok := occupant.(*WildTenant); ok && !wt.disappeared && !wt.moving {
        moveOutResp := make(chan interface{})
        board[newPos.X][newPos.Y] <- FieldRequest{
            Type:     MoveOutRequest,
            Response: moveOutResp,
        }

        if (<-moveOutResp).(bool) {
            timeout := time.After(time.Duration(MoveTimeout * float64(time.Second)))
            ticker := time.NewTicker(5 * time.Millisecond)
            defer ticker.Stop()

            for {
                select {
                case <-timeout:
                    return false
                case <-ticker.C:
                    checkResp := make(chan interface{})
                    board[newPos.X][newPos.Y] <- FieldRequest{
                        Type:     CheckOccupant,
                        Response: checkResp,
                    }
                    if (<-checkResp) == nil {
                        setResp := make(chan interface{})
                        board[newPos.X][newPos.Y] <- FieldRequest{
                            Type:        SetOccupant,
                            NewOccupant: t,
                            Response:    setResp,
                        }
                        if success := (<-setResp).(bool); success {
                            leaveResp := make(chan interface{})
                            board[t.Position.X][t.Position.Y] <- FieldRequest{
                                Type:     Leave,
                                Response: leaveResp,
                            }
                            <-leaveResp

                            t.Position = newPos
                            return true
                        }
                    }
                }
            }
        }
    }

    return false
}

func travelerRoutine(t *Traveler, reportChannel chan TraceSequence, board [][]chan FieldRequest) {
	defer wg.Done()
	r := rand.New(rand.NewSource(t.Seed))
	var localTraces []Trace

	for {
		x := r.Intn(BoardWidth)
		y := r.Intn(BoardHeight)
		resp := make(chan interface{})
		board[x][y] <- FieldRequest{
			Type:       SetOccupant,
			NewOccupant: t,
			Response:   resp,
		}
		
		if (<-resp).(bool) {
			t.Position = Position{x, y}
			break
		}
	}

	localTraces = append(localTraces, Trace{
		Timestamp: time.Since(startTime),
		ID:        t.ID,
		Position:  t.Position,
		Symbol:    t.Symbol,
	})

	for i := 0; i < t.Steps && !t.Dead; i++ {
		time.Sleep(time.Duration(MinDelay*float64(time.Second)) + time.Duration(r.Float64()*MaxDelay*float64(time.Second)))

		moved := false
		for attempts := 0; attempts < 4; attempts++ {
			newPos := moveDirection(t.Position, r.Intn(4))
			if t.tryMove(newPos, board) {
				localTraces = append(localTraces, Trace{
					Timestamp: time.Since(startTime),
					ID:        t.ID,
					Position:  t.Position,
					Symbol:    t.Symbol,
				})
				moved = true
				break
			}
		}
		if !moved {
			t.Dead = true
			t.Symbol = rune(int(t.Symbol) + 32)
			localTraces = append(localTraces, Trace{
				Timestamp: time.Since(startTime),
				ID:        t.ID,
				Position:  t.Position,
				Symbol:    t.Symbol,
			})
			break
		}
	}

	reportChannel <- TraceSequence{Traces: localTraces}
}

func wildTenantRoutine(wt *WildTenant, reportChannel chan TraceSequence, board [][]chan FieldRequest) {
	defer wg.Done()
	r := rand.New(rand.NewSource(time.Now().UnixNano()))
	var localTraces []Trace

	// Losowe czekanie zanim się pojawi
	time.Sleep(wt.TimeBegin)

	for {
		x := r.Intn(BoardWidth)
		y := r.Intn(BoardHeight)
		resp := make(chan interface{})
		board[x][y] <- FieldRequest{
			Type:       SetOccupant,
			NewOccupant: wt,
			Response:   resp,
		}
		
		if (<-resp).(bool) {
			wt.Position = Position{x, y}
			break
		}
	}

	localTraces = append(localTraces, Trace{
		Timestamp: time.Since(startTime),
		ID:        wt.ID,
		Position:  wt.Position,
		Symbol:    wt.Symbol,
	})

	reportChannel <- TraceSequence{Traces: localTraces}

	timeout := time.After(wt.TimeEnd)
	
	for {
		select {
		case <-wt.MoveOutChan:
			// Try to move to adjacent field
			for _, dir := range rand.Perm(4) {
				possiblePos := moveDirection(wt.Position, dir)
				
				resp := make(chan interface{})
				board[possiblePos.X][possiblePos.Y] <- FieldRequest{
					Type:     CheckOccupant,
					Response: resp,
				}
				
				if (<-resp) == nil {
					// Try to acquire new position
					setResp := make(chan interface{})
					board[possiblePos.X][possiblePos.Y] <- FieldRequest{
						Type:       SetOccupant,
						NewOccupant: wt,
						Response:   setResp,
					}
					
					if (<-setResp).(bool) {
						// Release old position
					leaveResp := make(chan interface{})
					board[wt.Position.X][wt.Position.Y] <- FieldRequest{
					Type:     Leave,
					Response: leaveResp,
				}
				<-leaveResp

						
						wt.Position = possiblePos
						localTraces = append(localTraces, Trace{
							Timestamp: time.Since(startTime),
							ID:        wt.ID,
							Position:  wt.Position,
							Symbol:    wt.Symbol,
						})
						reportChannel <- TraceSequence{Traces: localTraces}
						break
					}
				}
			}

		case <-timeout:
			leaveResp := make(chan interface{})
			board[wt.Position.X][wt.Position.Y] <- FieldRequest{
				Type:     Leave,
				Response: leaveResp,
			}
			<-leaveResp
			
			wt.disappeared = true
			
			reportChannel <- TraceSequence{Traces: []Trace{
				{
					Timestamp: time.Since(startTime),
					ID:        wt.ID,
					Position:  Position{X: BoardWidth, Y: BoardHeight},
					Symbol:    wt.Symbol,
				},
			}}
			return

		case <-wt.StopChan:
			leaveResp := make(chan interface{})
board[wt.Position.X][wt.Position.Y] <- FieldRequest{
	Type:     Leave,
	Response: leaveResp,
}
<-leaveResp

			wt.disappeared = true
			
			reportChannel <- TraceSequence{Traces: []Trace{
				{
					Timestamp: time.Since(startTime),
					ID:        wt.ID,
					Position:  Position{X: BoardWidth, Y: BoardHeight},
					Symbol:    wt.Symbol,
				},
			}}
			return
		}
	}
}

func Print_Trace(trace Trace) {
	fmt.Printf(
		" %.9f  %d  %d  %d  %c\n",
		trace.Timestamp.Seconds(),
		trace.ID,
		trace.Position.X,
		trace.Position.Y,
		trace.Symbol,
	)
}

func printer(reportChannel chan TraceSequence, done chan struct{}) {
	for {
		select {
		case traceSeq, ok := <-reportChannel:
			if !ok {
				close(done)
				return
			}
			for _, trace := range traceSeq.Traces {
				Print_Trace(trace)
			}
		case <-done:
			return
		}
	}
}

func main() {
    board := initBoard() 
    rand.Seed(time.Now().UnixNano())
    fmt.Printf("-1  %d  %d  %d\n", NrOfAll, BoardWidth, BoardHeight)

    reportChannel := make(chan TraceSequence, NrOfAll*MaxSteps)
    done := make(chan struct{})

    go printer(reportChannel, done)

    for i := 0; i < NrOfTravelers; i++ {
        wg.Add(1)
        t := &Traveler{
            ID:     i,
            Symbol: rune('A' + i),
            Steps:  MinSteps + rand.Intn(MaxSteps-MinSteps),
            Seed:   time.Now().UnixNano(),
        }
        go travelerRoutine(t, reportChannel, board)
    }

    for i := 0; i < NrOfWildTenants; i++ {
        wg.Add(1)
        wt := &WildTenant{
            ID:          i + NrOfTravelers,
            Symbol:      rune('0' + i),
            TimeBegin:   time.Duration(rand.Float64() * WildTenantMinDur * float64(time.Second)),
            TimeEnd:     time.Duration((WildTenantMinDur + rand.Float64()*(2.5-WildTenantMinDur)) * float64(time.Second)),
            StopChan:    make(chan struct{}),
            MoveOutChan: make(chan struct{}, 1),
        }
        go wildTenantRoutine(wt, reportChannel, board)
    }

    wg.Wait()
    
    for i := range board {
        for j := range board[i] {
            close(board[i][j])
        }
    }
    
    close(reportChannel)
    <-done
}