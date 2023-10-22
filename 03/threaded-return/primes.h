#ifndef PRIMES_H
#define PRIMES_H

#include <cmath>

/*
    Count prime numbers up to N
     * Optional parameters:
        start   -- first number to check
        skip    -- distance to next number
*/

long prime_count (long N, long start=3, long skip=2)
{
    long total = 0;
    for (long i = start; i <= N; i += skip) // Loop over selected numbers
    {
        total += 1;                         // Add as a potential prime
        float sqi = sqrt(i);                // Calculate largest divisor
        for (long j = 3; j <= sqi; j += 2)  // Loop over possible divisors
        {
            if (i%j == 0)
            {
                total -= 1; break;      // Discard if divisor is found
            }
        }
    }
    return total;
}

#endif // PRIMES_H
