package main

import (
	"fmt"
	"math/rand"
	"os"
	"sync"
	"sync/atomic"
	"time"
)

type Logger struct {
	mu        sync.Mutex
	startTime time.Time
}

func NewLogger() *Logger {
	return &Logger{
		startTime: time.Now(),
	}
}

func (l *Logger) getTimestamp() string {
	elapsed := time.Since(l.startTime).Milliseconds()
	return fmt.Sprintf("[%6dms]", elapsed)
}

func (l *Logger) Log(philosopherId int, action string) {
	l.mu.Lock()
	defer l.mu.Unlock()
	fmt.Printf("%s Filozof%d: %s\n", l.getTimestamp(), philosopherId, action)
}

type Fork struct {
	mu              sync.Mutex
	isClean         bool
	ownerId         int
	waitingPriority int32 
}

func NewFork() *Fork {
	return &Fork{
		isClean:         true,
		ownerId:         -1,
		waitingPriority: -1,
	}
}

func (f *Fork) IsClean() bool {
	return f.isClean
}

func (f *Fork) SetDirty() {
	f.isClean = false
}

func (f *Fork) SetClean() {
	f.isClean = true
}

func (f *Fork) TryTake(philosopherId int, wasDirty *bool) bool {
	if f.mu.TryLock() {
		*wasDirty = !f.isClean
		f.ownerId = philosopherId
		f.waitingPriority = -1
		return true
	}
	return false
}
func (f *Fork) TryTakeWithPriority(philosopherId int, hungerLevel int32, wasDirty *bool) bool {
	if atomic.LoadInt32(&f.waitingPriority) > hungerLevel {
		return false
	}

	if f.mu.TryLock() {
		*wasDirty = !f.isClean
		f.ownerId = philosopherId
		atomic.StoreInt32(&f.waitingPriority, -1)
		return true
	}

	current := atomic.LoadInt32(&f.waitingPriority)
	if hungerLevel > current {
		atomic.CompareAndSwapInt32(&f.waitingPriority, current, hungerLevel)
	}
	return false
}

// Take - blokujące branie widelca
func (f *Fork) Take(philosopherId int, wasDirty *bool) {
	f.mu.Lock()
	*wasDirty = !f.isClean
	f.ownerId = philosopherId
	atomic.StoreInt32(&f.waitingPriority, -1)
}

// Put - zwolnienie widelca
func (f *Fork) Put(philosopherId int) {
	f.isClean = false
	f.ownerId = -1
	atomic.StoreInt32(&f.waitingPriority, -1)
	f.mu.Unlock()
}

// Philosopher - filozof
type Philosopher struct {
	id                   int
	mealsToEat           int32
	failedMeals          *int32
	forks                []*Fork
	logger               *Logger
	thinkDist            [2]int
	eatDist              [2]int
	consecutiveFailures  int32
	totalFailedAttempts  int32
}

func NewPhilosopher(id int, mealsToEat int32, failedMeals *int32, forks []*Fork, logger *Logger) *Philosopher {
	return &Philosopher{
		id:                  id,
		mealsToEat:          mealsToEat,
		failedMeals:         failedMeals,
		forks:               forks,
		logger:              logger,
		thinkDist:           [2]int{50, 200},
		eatDist:             [2]int{100, 300},
		consecutiveFailures: 0,
		totalFailedAttempts: 0,
	}
}

func (p *Philosopher) getHungerLevel() int32 {
	level := p.consecutiveFailures * 10
	if level > 100 {
		level = 100
	}
	return level
}

func (p *Philosopher) think() {
	baseTime := p.thinkDist[0] + rand.Intn(p.thinkDist[1]-p.thinkDist[0])
	thinkTime := baseTime

	if p.consecutiveFailures > 3 {
		thinkTime = baseTime / 2
		p.logger.Log(p.id, fmt.Sprintf("Myśli... (GŁODNY! czas skrócony do %dms)", thinkTime))
	} else if p.consecutiveFailures > 0 {
		thinkTime = baseTime * 7 / 10
		p.logger.Log(p.id, fmt.Sprintf("Myśli... (lekko głodny, czas %dms)", thinkTime))
	} else {
		p.logger.Log(p.id, fmt.Sprintf("Myśli... (spokojnie, czas %dms)", thinkTime))
	}

	time.Sleep(time.Duration(thinkTime) * time.Millisecond)
}

func (p *Philosopher) eat(mealsEaten int32) {
	p.logger.Log(p.id, fmt.Sprintf("JE OBIAD! (posiłek %d/%d)", mealsEaten+1, p.mealsToEat))
	eatTime := p.eatDist[0] + rand.Intn(p.eatDist[1]-p.eatDist[0])
	time.Sleep(time.Duration(eatTime) * time.Millisecond)
}

func (p *Philosopher) takeForks() bool {
	N := len(p.forks)
	leftFork := p.id
	rightFork := (p.id + 1) % N

	firstFork := min(leftFork, rightFork)
	secondFork := max(leftFork, rightFork)

	var wasDirty1, wasDirty2 bool
	hungerLevel := p.getHungerLevel()

	p.forks[firstFork].Take(p.id, &wasDirty1)

	timeout := 100 + (int(p.consecutiveFailures) * 20)
	if timeout > 500 {
		timeout = 500
	}

	if p.consecutiveFailures > 5 {
		p.logger.Log(p.id, fmt.Sprintf("OSTRZEŻENIE: Bardzo głodny! timeout=%dms", timeout))
	}

	start := time.Now()
	attempts := 0

	for time.Since(start).Milliseconds() < int64(timeout) {
		var success bool

		if p.consecutiveFailures > 2 {
			success = p.forks[secondFork].TryTakeWithPriority(p.id, hungerLevel, &wasDirty2)
		} else {
			success = p.forks[secondFork].TryTake(p.id, &wasDirty2)
		}

		if success {
			if wasDirty1 {
				p.forks[firstFork].SetClean()
			}
			if wasDirty2 {
				p.forks[secondFork].SetClean()
			}

			atomic.StoreInt32(&p.consecutiveFailures, 0)

			p.logger.Log(p.id, fmt.Sprintf("Udało się wziąć widelce %d i %d (po %d próbach)", firstFork, secondFork, attempts))
			return true
		}

		attempts++
		if attempts%5 == 0 && p.consecutiveFailures > 2 {
			p.logger.Log(p.id, fmt.Sprintf("Ciągle czekam na widelec %d... (próba %d)", secondFork, attempts))
		}

		time.Sleep(10 * time.Millisecond)
	}

	p.forks[firstFork].Put(p.id)
	atomic.AddInt32(&p.consecutiveFailures, 1)
	atomic.AddInt32(&p.totalFailedAttempts, 1)
	atomic.AddInt32(p.failedMeals, 1)

	p.logger.Log(p.id, fmt.Sprintf("Nie udało się wziąć widelców (głód poziom %d, timeout %dms)", 
		atomic.LoadInt32(&p.consecutiveFailures), timeout))
	return false
}

func (p *Philosopher) Dine() {
	var mealsEaten int32 = 0

	for mealsEaten < p.mealsToEat {
		p.think()

		if p.takeForks() {
			p.eat(mealsEaten)

			N := len(p.forks)
			leftFork := p.id
			rightFork := (p.id + 1) % N

			p.forks[rightFork].Put(p.id)
			p.forks[leftFork].Put(p.id)
			mealsEaten++

			p.logger.Log(p.id, fmt.Sprintf("Skończył posiłek %d/%d (łącznie głodował %d razy)", 
				mealsEaten, p.mealsToEat, atomic.LoadInt32(&p.totalFailedAttempts)))
		} else {
			p.logger.Log(p.id, fmt.Sprintf("GŁODUJE! (nie udało się %d razy)", atomic.LoadInt32(p.failedMeals)))
		}
	}

	p.logger.Log(p.id, fmt.Sprintf("ZAKOŃCZYŁ WSZYSTKIE POSIŁKI! (głodował %d razy)", 
		atomic.LoadInt32(&p.totalFailedAttempts)))
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func main() {
	numOfPhilosophers := 5
	mealsPerPhilosopher := 5

	if len(os.Args) == 3 {
		fmt.Sscan(os.Args[1], &numOfPhilosophers)
		fmt.Sscan(os.Args[2], &mealsPerPhilosopher)
	} else {
		fmt.Println("./zad1 <numOfPhilosophers> <mealsPerPhilosopher>")
		fmt.Println("Przykład: ./zad1 7 3")
		return
	}


	rand.Seed(time.Now().UnixNano())

	fmt.Printf("\n=== PROBLEM UCZTUJĄCYCH FILOZOFÓW ===\n")
	fmt.Printf("Liczba filozofów: %d\n", numOfPhilosophers)
	fmt.Printf("Posiłków na filozofa: %d\n", mealsPerPhilosopher)
	fmt.Printf("Mechanizm: czyste/brudne widelce + priorytet dla głodujących\n")
	fmt.Printf("======================================\n\n")

	logger := NewLogger()
	var wg sync.WaitGroup

	failedMeals := make([]int32, numOfPhilosophers)
	forks := make([]*Fork, numOfPhilosophers)
	for i := 0; i < numOfPhilosophers; i++ {
		forks[i] = NewFork()
	}

	for i := 0; i < numOfPhilosophers; i++ {
		wg.Add(1)
		go func(id int) {
			defer wg.Done()
			philosopher := NewPhilosopher(id, int32(mealsPerPhilosopher), &failedMeals[id], forks, logger)
			philosopher.Dine()
		}(i)
	}

	wg.Wait()

	fmt.Printf("\n=========================================\n")
	fmt.Printf("PODSUMOWANIE:\n")
	totalFailed := int32(0)
	for i := 0; i < numOfPhilosophers; i++ {
		fmt.Printf("Filozof %d: nie udało się zjeść %d razy\n", i, failedMeals[i])
		totalFailed += failedMeals[i]
	}

	fmt.Printf("\nCzy system był sprawiedliwy? ")
	maxFailed := int32(0)
	minFailed := int32(mealsPerPhilosopher)
	for i := 0; i < numOfPhilosophers; i++ {
		if failedMeals[i] > maxFailed {
			maxFailed = failedMeals[i]
		}
		if failedMeals[i] < minFailed {
			minFailed = failedMeals[i]
		}
	}

	if maxFailed-minFailed <= 1 {
		fmt.Printf("TAK  - wszyscy filozofowie głodowali podobnie często\n")
	} else {
		fmt.Printf("NIE  - filozofowie głodowali z różną częstotliwością (różnica %d)\n", maxFailed-minFailed)
	}

	fmt.Printf("Łączna liczba nieudanych prób: %d\n", totalFailed)
	fmt.Printf("Średnia nieudanych prób na filozofa: %d\n", totalFailed/int32(numOfPhilosophers))
	fmt.Printf("=========================================\n")
}