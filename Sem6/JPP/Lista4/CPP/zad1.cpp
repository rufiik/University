#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <atomic>
#include <random>

using namespace std;
using namespace chrono;

class Logger
{
private:
    mutex mtx_log;
    steady_clock::time_point startTime;

    string getTimeStamp()
    {
        auto now = steady_clock::now();
        auto elapsed = duration_cast<milliseconds>(now - startTime);
        stringstream ss;
        ss << "[" << setw(6) << elapsed.count() << "ms]";
        return ss.str();
    }

public:
    Logger() : startTime(steady_clock::now()) {};

    void log(int philosopherId, const string &action)
    {
        lock_guard<mutex> lock(mtx_log);
        cout << getTimeStamp() << " Filozof" << philosopherId << ": " << action << "\n";
    }
};

class Fork
{
private:
    mutex mtx;
    bool isClean;
    int ownerId;

public:
    Fork() : isClean(true), ownerId(-1) {}
    bool isForkClean() const { return isClean; }
    void setDirty() { isClean = false ;}
    void setClean() { isClean = true; }

    bool tryTake(int philosopherId, bool &wasDirty)
    {
        if (mtx.try_lock())
        {
            wasDirty = !isClean;
            ownerId = philosopherId;
            return true;
        }
        return false;
    }
    void take(int philosopherId, bool &wasDirty)
    {
        mtx.lock();
        wasDirty = !isClean;
        ownerId = philosopherId;
    }
    void put(int philosopherId)
    {
        isClean = false;
        ownerId = -1;
        mtx.unlock();
    }
};

class Philosopher
{
private:
    int id;
    int mealsToEat;
    atomic<int> &failedMeals;
    vector<Fork> &forks;
    Logger &logger;
    random_device rd;
    mt19937 gen;
    uniform_int_distribution<> thinkDist;
    uniform_int_distribution<> eatDist;

public:
    void think()
    {
        logger.log(id, "Myśli...");
        this_thread::sleep_for(milliseconds(thinkDist(gen)));
    }
    void eat()
    {
        logger.log(id, "JE OBIAD! (numer " + to_string(mealsToEat - failedMeals.load()) + ")");
        this_thread::sleep_for(milliseconds(eatDist(gen)));
    }

    bool takeForks()
    {
        int firstFork = id;
        int secondFork = (id + 1) % forks.size();

        if (firstFork > secondFork)
        {
            swap(firstFork, secondFork);
        }

        bool wasDirty1, wasDirty2;

        forks[firstFork].take(id, wasDirty1);
        auto start = steady_clock::now();
        while (duration_cast<milliseconds>(steady_clock::now() - start).count() < 100)
        {
            if (forks[secondFork].tryTake(id, wasDirty2))
            {
                if (wasDirty1)
                    forks[firstFork].setClean();
                if (wasDirty2)
                    forks[secondFork].setClean();

                logger.log(id, "Udało się wziąć widelce " + to_string(firstFork) +
                                   " i " + to_string(secondFork));
                return true;
            }
            this_thread::sleep_for(milliseconds(10));
        }

        forks[firstFork].put(id);
        logger.log(id, "Nie udało się wziąć widelców");
        return false;
    }

public:
    Philosopher(int i, int meals, atomic<int> &failed, vector<Fork> &f, Logger &l) : id(i), mealsToEat(meals), failedMeals(failed), forks(f), logger(l), gen(rd()), thinkDist(50, 200), eatDist(100, 300)
    {
        failedMeals = 0;
    }
    void dine()
    {
        int mealsEaten = 0;
        while (mealsEaten < mealsToEat)
        {
            think();
            if (takeForks())
            {
                eat();
                int firstFork = id;
                int secondFork = (id + 1) % forks.size();

                forks[secondFork].put(id);
                forks[firstFork].put(id);
                mealsEaten++;

                logger.log(id, "Skończył posiłek " + to_string(mealsEaten) + "/" + to_string(mealsToEat));
            }
            else
            {
                failedMeals++;
                logger.log(id, "GŁODUJE! (nie udało się " + to_string(failedMeals.load()) + " razy)");
            }
        }
        logger.log(id, "Skończył wszystkie posiłki");
    }
};

int main(int argc, char *argv[])
{
    int numOfPhilosophers = 5;
    int mealsPerPhilosopher = 5;

    if (argc == 3)
    {
        numOfPhilosophers = stoi(argv[1]);
        mealsPerPhilosopher = stoi(argv[2]);
    }
    else
    {
        cout << "./zad1 <numOfPhilosophers> <mealsPerPhilosopher> \n";
        return 0;
    }

    cout << "Liczba filozofów " << numOfPhilosophers << "\n";
    cout << "Liczba posiłków na filozofa " << mealsPerPhilosopher << "\n";

    Logger logger;
    vector<thread> philosophers;
    vector<atomic<int>> failedMeals(numOfPhilosophers);
    vector<Fork> forks(numOfPhilosophers);
    for (int i = 0; i < numOfPhilosophers; i++)
    {
        philosophers.emplace_back([i, mealsPerPhilosopher, &failedMeals, &forks, &logger]()
                                  {
                Philosopher p(i, mealsPerPhilosopher, failedMeals[i], forks, logger);
                p.dine(); });
    }
    for (auto& p : philosophers) {
        p.join();
    }
 // Podsumowanie
    cout << "\n=====================================" << endl;
    cout << "PODSUMOWANIE:" << endl;
    int totalFailed = 0;
    for (int i = 0; i < numOfPhilosophers; i++) {
        cout << "Filozof " << i << ": nie udało się zjeść " << failedMeals[i].load() << " razy" << endl;
        totalFailed += failedMeals[i].load();
    }
    
    cout << "\nCzy system był sprawiedliwy? ";
    int maxFailed = 0, minFailed = mealsPerPhilosopher;
    for (int i = 0; i < numOfPhilosophers; i++) {
        maxFailed = max(maxFailed, failedMeals[i].load());
        minFailed = min(minFailed, failedMeals[i].load());
    }
    
    if (maxFailed - minFailed <= 1) {
        cout << "TAK - wszystkie filozofy głodowały podobnie często" << endl;
    } else {
        cout << "NIE - filozofowie głodowali z różną częstotliwością (różnica " 
             << (maxFailed - minFailed) << ")" << endl;
    }
    
    cout << "Łączna liczba nieudanych prób: " << totalFailed << endl;
    cout << "=====================================" << endl;
    
    return 0;
}