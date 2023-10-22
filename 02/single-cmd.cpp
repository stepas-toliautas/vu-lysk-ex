#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "primes.h"

void worker (long N, long start, long skip);

int main (int argc, char** argv)
{
    /* Check command-line arguments */
    long N = 1e7;           if (argc>1) N = atol(argv[1]);
    long start = 1;         if (argc>2) start = atol(argv[2]);
    long skip = 2;          if (argc>3) skip = atol(argv[3]);
    /* Do not print output if called with arguments */
    if (argc==1) printf ("Single program (worker): calculating...\n");
    long t = clock();
    worker(N, start, skip);
    t = clock() - t;
    if (argc==1) printf ("Calculated in %.2f s\n", ((float)t)/CLOCKS_PER_SEC);
}

void worker (long N, long start, long skip)
{
    long primes = prime_count(N, start, skip);
    printf ("Primes in an interval [%g:%ld:%g]: %ld\n",
            (double)start, skip, (double)N, primes);
}
