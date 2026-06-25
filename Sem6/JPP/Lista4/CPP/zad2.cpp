#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>
#include <atomic>
#include <deque>
#include <sstream>

using namespace std;

struct Message {
    int from;
    int to;
    int msg_id;
};

void safe_print(const string& msg) {
    static mutex cout_mtx;
    lock_guard<mutex> lock(cout_mtx);
    cout << msg << endl;
}

class User {
public:
    int id;
    queue<Message> incoming;
    mutex mtx;
    condition_variable cv;
    int sent_count = 0;
    int received_count = 0;

    User(int i) : id(i) {}
    
    User(const User&) = delete;
    User& operator=(const User&) = delete;
};

class Server {
private:
    deque<User>& users;
    mutex mtx;
    condition_variable cv_to_server;  
    condition_variable cv_from_server; 
    
    bool busy = false;
    bool msg_available = false;
    Message current_msg;
    atomic<bool> running{true};

public:
    Server(deque<User>& u) : users(u) {}

    void send_to_server(Message msg) {
        unique_lock<mutex> lock(mtx);
        
        cv_to_server.wait(lock, [this] { return !busy || !running; });
        if (!running) return;

        current_msg = msg;
        msg_available = true;
        busy = true;

        stringstream ss;
        ss << "[SERVER] Odebrano od U" << msg.from << " dla U" << msg.to << " (ID: " << msg.msg_id << ")";
        safe_print(ss.str());
        
        cv_from_server.notify_one();
        
        cv_to_server.wait(lock, [this] { return !busy || !running; });
    }

    void run() {
        while (running) {
            unique_lock<mutex> lock(mtx);
            cv_from_server.wait(lock, [this] { return msg_available || !running; });

            if (!running) break;

            Message msg = current_msg;
            msg_available = false;

            {
                lock_guard<mutex> user_lock(users[msg.to].mtx);
                users[msg.to].incoming.push(msg);
                users[msg.to].received_count++;
                users[msg.to].cv.notify_one();
            }

            stringstream ss;
            ss << "[SERVER] Wiadomość ID:" << msg.msg_id << " dostarczona do U" << msg.to;
            safe_print(ss.str());

            busy = false; 
            cv_to_server.notify_all(); 
        }
    }

    void stop() {
        running = false;
        cv_to_server.notify_all();
        cv_from_server.notify_all();
    }
};

void sender_func(User& me, Server& server, int num_msg, int num_users) {
    mt19937 gen(me.id + time(0));
    uniform_int_distribution<> dist(0, num_users - 1);

    for (int i = 0; i < num_msg; ++i) {
        int recipient = dist(gen);
        Message msg{me.id, recipient, i};
        
        stringstream ss;
        ss << "[U" << me.id << "] Próbuje wysłać wiadomość ID:" << i << " do U" << recipient;
        safe_print(ss.str());

        server.send_to_server(msg);
        me.sent_count++;
        
        this_thread::sleep_for(chrono::milliseconds(20)); 
    }
}

void receiver_func(User& me) {
    while (true) {
        unique_lock<mutex> lock(me.mtx);
        if (me.cv.wait_for(lock, chrono::seconds(2), [&me] { return !me.incoming.empty(); })) {
            while (!me.incoming.empty()) {
                Message msg = me.incoming.front();
                me.incoming.pop();
                
                stringstream ss;
                ss << "         [U" << me.id << "] OTRZYMAŁEM wiadomość ID:" << msg.msg_id << " od U" << msg.from;
                safe_print(ss.str());
            }
        } else {
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    int num_users = 5;
    int messages_per_user = 5;

    if (argc == 3) {
        num_users = stoi(argv[1]);
        messages_per_user = stoi(argv[2]);
    } else {
        cout << "Użycie: " << argv[0] << " <użytkownicy> <wiadomości>" << endl;
        cout << "Uruchamiam z domyślnymi parametrami: 5 użytkowników, 5 wiadomości.\n" << endl;
    }

    deque<User> users;
    for (int i = 0; i < num_users; ++i) users.emplace_back(i);

    Server server(users);
    thread s_thread(&Server::run, &server);

    vector<thread> senders;
    vector<thread> receivers;

    for (int i = 0; i < num_users; ++i) {
        senders.emplace_back(sender_func, ref(users[i]), ref(server), messages_per_user, num_users);
        receivers.emplace_back(receiver_func, ref(users[i]));
    }

    for (auto& t : senders) t.join();
    
    server.stop();
    s_thread.join();
    for (auto& t : receivers) t.join();

    cout << "STATYSTYKI KOŃCOWE\n";
    
    int total_sent = 0;
    int total_received = 0;

    for (const auto& u : users) {
        cout << "Użytkownik " << u.id << ": Wysłano " << u.sent_count 
             << " | Odebrano " << u.received_count << endl;
        total_sent += u.sent_count;
        total_received += u.received_count;
    }

    cout << "\nŁącznie wysłanych:  " << total_sent << endl;
    cout << "Łącznie odebranych: " << total_received << endl;

    if (total_sent == total_received) {
        cout << "SUKCES: Wszystkie komunikaty dotarły." << endl;
    } else {
        cout << "BŁĄD: Wykryto stratę komunikatów!" << endl;
    }

    return 0;
}