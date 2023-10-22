#include <cstdio>
#include <ctime>
#include <thread>
#include <mutex>
#include "primes.h"
using namespace std;    // std::thread etc.


void worker (long N, long start, long skip);
void threaded_prime_count (long N);

const int WORKERS = 4;
long total = 0;             /// Global variable
mutex my_res;               /// Lock for access to 'total'



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
    { /// Add local scope
        lock_guard<mutex> lock(my_res);                 /// Acquire lock
        total += primes;
        printf ("Primes in an interval [%g:%ld:%g]: %ld\n",
                (double)start, skip, (double)N, primes);
    } /// Output lock is automatically released at the end of local scope
}
