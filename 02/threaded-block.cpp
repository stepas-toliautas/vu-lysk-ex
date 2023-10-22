#include <cstdio>
#include <ctime>
#include <thread>
#include "primes.h"
using namespace std;    // std::thread etc.

void worker (long N, long start, long skip);
void threaded_prime_count (long N);

const int WORKERS = 3;

int main ()
{
    long N = 1e8;
    printf ("C++11 threads (%d): calculating...\n", WORKERS);
    long t = clock();
    threaded_prime_count(N);
    t = clock() - t;
    printf ("Calculated in %.2f s\n", ((float)t)/CLOCKS_PER_SEC);
}

void threaded_prime_count (long N)
{
    thread tid[WORKERS];
    for (int i=0; i<WORKERS; ++i)
    {
        /* Prepare worker arguments */
        long Ni = (i+1)*N/WORKERS;              // Last number to check
        long fi = i*N/WORKERS;                  // First number to check
        long si = 2;                            // Skip even numbers
        /* Create thread and execute worker */
        tid[i] = thread(worker, Ni, fi|1, si);  // Start from odd number
    }
    /* Wait for all threads to end */
    for (int i=0; i<WORKERS; ++i)
        tid[i].join();
}

void worker (long N, long start, long skip)
{
    long primes = prime_count(N, start, skip);
    printf ("Primes in an interval [%g:%ld:%g]: %ld\n",
            (double)start, skip, (double)N, primes);
}
