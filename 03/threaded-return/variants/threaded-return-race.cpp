#include <cstdio>
#include <ctime>
#include <thread>
#include "primes.h"
using namespace std;    // std::thread etc.

void worker (long N, long start, long skip, long* result);
long threaded_prime_count (long N);

const int WORKERS = 4;

int main ()
{
    long N = 10*1000*1000;
    printf ("C++11 threads (%d): calculating...\n", WORKERS);
    long t = clock();
    long total = threaded_prime_count(N);
    t = clock() - t;
    printf ("Primes in an interval [1:%g]: %ld\n", (double)N, total);
    printf ("Calculated in %.2f s\n", ((float)t)/CLOCKS_PER_SEC);
}

long threaded_prime_count (long N)
{
    long total = 0, tmp = 0;                    // Results from each worker
    thread tid[WORKERS];
    for (int i=0; i<WORKERS; ++i)
    {
        /* Prepare worker arguments */
        long Ni = N;                            // Last number to check
        long fi = 1+2*i;                        // First number to check
        long si = 2*WORKERS;                    // Skip even numbers
        /* Create thread and execute worker */
        tid[i] = thread(worker, Ni, fi|1, si, &tmp);  // Pass output address
    }
    /* Wait for all threads to end */
    for (int i=0; i<WORKERS; ++i)
    {
        tid[i].join();
        total += tmp;                           // Add subtotal of each thread
    }
    return total;                               // Return number of all found primes
}

void worker (long N, long start, long skip, long* primes)
{
    *primes = prime_count(N, start, skip);
    printf ("Primes in an interval [%g:%ld:%g]: %ld\n",
            (double)start, skip, (double)N, *primes);
}
