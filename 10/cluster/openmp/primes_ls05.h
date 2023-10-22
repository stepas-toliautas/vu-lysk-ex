#ifndef PRIMES_LS05_H
#define PRIMES_LS05_H

#include <vector>
#include <string>

using fprime = long(*)(long);

/* DECLARATIONS */

long pc_serial    (long N);     long pc_naive     (long N);
long pc_critical  (long N);     long pc_atomic    (long N);
long pc_reduction (long N);
long pc_static_1  (long N);     long pc_static_4  (long N);
long pc_static16  (long N);     long pc_static64  (long N);
long pc_dynamic_1 (long N);     long pc_dynamic_4 (long N);
long pc_dynamic16 (long N);     long pc_dynamic64 (long N);
long pc_guided_1  (long N);     long pc_guided_4  (long N);
long pc_guided16  (long N);     long pc_guided64  (long N);
long pc_schedule  (long N);

const std::vector<fprime> prime_count = {     pc_serial,
    pc_naive,     pc_critical,  pc_atomic,    pc_reduction,
    pc_static_1,  pc_static_4,  pc_static16,  pc_static64,
    pc_dynamic_1, pc_dynamic_4, pc_dynamic16, pc_dynamic64,
    pc_guided_1,  pc_guided_4,  pc_guided16,  pc_guided64,
    pc_schedule,  };

const std::vector<std::string> prime_name = { "serial",
    "naive",     "critical",   "atomic",      "reduction",
    "static(1)", "static(4)",  "static(16)",  "static(64)",
    "dynamic",   "dynamic(4)", "dynamic(16)", "dynamic(64)",
    "guided",    "guided(4)",  "guided(16)",  "guided(64)",
    "schedule",  };

const unsigned slides[] = { 0, 1,   2, 3, 4,   5, 9, 13 };
const unsigned bench[]  = { 4,   5, 6, 7,   9, 10, 11,   13 };

/*
    IMPLEMENTATION
*/

#include <cmath>
#include <iostream>
#include <iomanip>
using namespace std;

/* REFERENCE */

long pc_serial (long N)
{
    long total = N/2;
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

/* SYNCHRONIZATION */

long pc_naive (long N)
{
    long total = N/2;
    #pragma omp parallel for
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_critical (long N)
{
    long total = N/2;
    #pragma omp parallel for
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                #pragma omp critical
                    total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_atomic (long N)
{
    long total = N/2;
    #pragma omp parallel for
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                #pragma omp atomic
                    total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_reduction (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) // schedule(static)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

/* SCHEDULING */

long pc_static_1 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(static,1)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_static_4 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(static,4)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_static16 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(static,16)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_static64 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(static,64)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_dynamic_1 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(dynamic,1)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_dynamic_4 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(dynamic,4)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_dynamic16 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(dynamic,16)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_dynamic64 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(dynamic,64)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_guided_1 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(guided,1)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_guided_4 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(guided,4)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_guided16 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(guided,16)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

long pc_guided64 (long N)
{
    long total = N/2;
    #pragma omp parallel for reduction(+:total) schedule(guided,64)
    for (long i = 3; i <= N; i += 2)
    {
        float sqi = sqrt(i);
        for (long j = 3; j <= sqi; j += 2)
        {
            if (i%j == 0)
            {
                total -= 1;
                break;
            }
        }
    }
    return total;
}

/* EXAMPLE W/ OUTPUT */

long pc_schedule (long N)
{
    long total = N/2;
    #pragma omp parallel
    {
        vector<long> work;
        #pragma omp for reduction(+:total) schedule(guided)
        for (long i = 3; i <= N; i += 2)
        {
            work.push_back(i);
            float sqi = sqrt(i);
            for (long j = 3; j <= sqi; j += 2)
            {
                if (i%j == 0)
                {
                    total -= 1;
                    break;
                }
            }
        }
        #pragma omp critical
        {
            cout << omp_get_thread_num() << ":  ";
            for (auto i : work)
                cout << setw(2) << i << " ";
            cout << endl;
        }
    }
    return total;
}

#endif // PRIMES_LS05_H
