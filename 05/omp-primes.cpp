#include <iostream>
#include <iomanip>
#include <omp.h>
#include "primes_omp.h"
using namespace std;

double launch (int WORKERS, unsigned fi, long N, bool output = true);

int main()
{
    // Examples from lecture slides
    cout << "a) LS05: SLIDE EXAMPLES\n\n";
    for (auto i : slides)
        { launch (4, i, 1e7); cout << endl; }
    // Visual scheduling example
    cout << "b) LS05: EXAMPLE OF SCHEDULING RESULTS\n\n";
    launch (3, prime_count.size()-1, 100); cout << endl;
    // Speed comparison of scheduling algorithms
    cout << "c) LS05: SCHEDULING TYPE BENCHMARK\n\n";
/*
    HW 5.1: compare computation speed using different synchronization types
            and varying numbers of worker threads
*/
}

double launch (int WORKERS, unsigned fi, long N, bool output)
{
    omp_set_num_threads(WORKERS);
    if (output) {
        cout << "OpenMP threads (" << WORKERS << "): ";
        cout << "using '" << prime_name[fi] << "'\n";
    }

    double t = omp_get_wtime();
    long primes = prime_count[fi](N);
    t = omp_get_wtime() - t;

    if (output) {
        cout << "Primes in an interval [1, " << (double)N << "]: ";
        cout << primes << endl;
        cout << "Calculated in " << t << " s\n";
    }
    return t;
}
