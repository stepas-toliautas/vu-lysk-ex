#include <cstdio>
#include <ctime>
#include "primes.h"


void worker (long N);

int main ()
{
    long N = 1e8;
    printf ("Single thread: calculating...\n");
    long t = clock();
    worker(N);
    t = clock() - t;
    printf ("Calculated in %.2f s\n", ((float)t)/CLOCKS_PER_SEC);
}

void worker (long N)
{
    long primes = prime_count(N) + 1; // Include number 2
    printf ("Primes in an interval [1:%g]: %ld\n", (double)N, primes);
}
