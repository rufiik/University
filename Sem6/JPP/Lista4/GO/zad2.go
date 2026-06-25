package main

import (
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"sync"
	"time"
)

type Message struct {
	from   int
	to     int
	msgID  int
}

type User struct {
	id            int
	inbox         chan Message
	sentCount     int
	receivedCount int
	done          chan bool
}

type Server struct {
	users []*User
}

func (s *Server) Transfer(msg Message) {
	fmt.Printf("[SERVER] Przesyłam: U%d -> U%d (ID: %d)\n", msg.from, msg.to, msg.msgID)
	
	s.users[msg.to].inbox <- msg
	
	fmt.Printf("[SERVER] Dostarczono do U%d\n", msg.to)
}

func main() {
	numUsers := 0
	msgPerUser := 0

	if len(os.Args) == 3 {
		u, err1 := strconv.Atoi(os.Args[1])
		m, err2 := strconv.Atoi(os.Args[2])
		if err1 != nil || err2 != nil {
			fmt.Println("Błąd: Argumenty muszą być liczbami całkowitymi.")
			os.Exit(1)
		}
		numUsers = u
		msgPerUser = m
	} else {
		fmt.Println("Użycie: program <liczba_użytkowników> <wiadomości_na_użytkownika>")
		os.Exit(1)
	}

	fmt.Printf("System: %d użytkowników, %d wiadomości na każdego\n\n", numUsers, msgPerUser)

	users := make([]*User, numUsers)
	for i := 0; i < numUsers; i++ {
		users[i] = &User{
			id:    i,
			inbox: make(chan Message), 
			done:  make(chan bool),
		}
	}

	server := &Server{users: users}
	
	serverMtx := sync.Mutex{}

	var wg sync.WaitGroup

	for i := 0; i < numUsers; i++ {
		wg.Add(1)
		go func(u *User) {
			defer wg.Done()
			timeout := time.After(2 * time.Second)
			for {
				select {
				case msg := <-u.inbox:
					u.receivedCount++
					fmt.Printf("         [U%d] Odebrałem ID:%d od U%d\n", u.id, msg.msgID, msg.from)
					timeout = time.After(2 * time.Second)
				case <-timeout:
					return
				}
			}
		}(users[i])
	}

	for i := 0; i < numUsers; i++ {
		wg.Add(1)
		go func(u *User) {
			defer wg.Done()
			r := rand.New(rand.NewSource(time.Now().UnixNano() + int64(u.id)))
			
			for m := 0; m < msgPerUser; m++ {
				target := r.Intn(numUsers)
				msg := Message{from: u.id, to: target, msgID: m}
				
				fmt.Printf("[U%d] Wysyłam ID:%d do U%d\n", u.id, m, target)
				
				serverMtx.Lock()
				server.Transfer(msg)
				serverMtx.Unlock()
				
				u.sentCount++
				time.Sleep(time.Millisecond * 20)
			}
		}(users[i])
	}

	wg.Wait()

	fmt.Println("STATYSTYKI KOŃCOWE")
	totalSent, totalReceived := 0, 0
	for _, u := range users {
		fmt.Printf("Użytkownik %d: Wysłano %d | Odebrano %d\n", u.id, u.sentCount, u.receivedCount)
		totalSent += u.sentCount
		totalReceived += u.receivedCount
	}
	fmt.Printf("\nŁącznie wysłano: %d\nŁącznie odebrano: %d\n", totalSent, totalReceived)
}