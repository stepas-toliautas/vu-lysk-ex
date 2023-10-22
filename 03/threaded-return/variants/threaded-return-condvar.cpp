#include <cstdio>
#include <ctime>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "primes.h"
using namespace std;    // std::thread etc.

void worker (long N, long start, long skip);
void threaded_prime_count (long N);

const int WORKERS = 4;
long total = 0;             /// Global variable
mutex my_res;               /// Lock for access to 'total'
int turn = 1;               /// Current 'turn' value for output
condition_variable my_turn; /// Object to wait on turn

int main ()
{
    long N = 10*1000*1000;
    printf ("C++11 threads (%d): calculating...\n", WORKERS);
    long t = clock();
    threaded_prime_count(N);
    t = clock() - t;
    printf ("Primes in an interval [1:%g]: %ld\n", (double)N, total);
    printf ("Calculated in %.2f s\n", ((float)t)/CLOCKS_PER_SEC);
}

void threaded_prime_count (long N)
{
    thread tid[WORKERS];
    for (int i=0; i<WORKERS; ++i)
    {
        /* Prepare worker arguments */
        long Ni = N;                            // Last number to check
        long fi = 1+2*i;                        // First number to check
        long si = 2*WORKERS;                    // Skip even numbers
        /* Create thread and execute worker */
        tid[i] = thread(worker, Ni, fi|1, si);
    }
    /* Wait for all threads to end */
    for (int i=0; i<WORKERS; ++i)
        tid[i].join();
}

void worker (long N, long start, long skip)
{
    long primes = prime_count(N, start, skip);
    unique_lock<mutex> lock(my_res);                    /// Acquire lock
    while (turn != start) my_turn.wait(lock);           /// Wait for own turn
    total += primes;
    printf ("Primes in an interval [%g:%ld:%g]: %ld\n",
            (double)start, skip, (double)N, primes);
    turn += 2; my_turn.notify_all();                    /// Wake up threads
    lock.unlock();                                      /// Release lock
}
