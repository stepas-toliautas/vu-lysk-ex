#include <iostream>
#include <cmath>
#include <omp.h>
#include "primes_ls05.h"
using namespace std;

const int MAX_WORKERS = 256;

int main ()
{
    // Ask user for thread count
    cout << "How many new threads you'd like to run?\n";
    int n; cin >> n;
    n = (n<1)?1:((n>MAX_WORKERS)?MAX_WORKERS:n);

    // Number of threads to execute
    omp_set_num_threads(n);

    cout << "------------------------\n";
    // Find out OpenMP date (version)
#ifdef _OPENMP
    cout << "OpenMP standard date:   " << _OPENMP << "\n";
#endif
    // Various OpenMP run-time functions
    cout << "Number of CPUs:         " << omp_get_num_procs() << endl;
    cout << "System thread limit:    " << omp_get_thread_limit() << endl;
    cout << "Active threads:         " << omp_get_num_threads();
    cout << " of " << omp_get_max_threads() << endl;

    cout << "Dynamic thread count is "
         << (omp_get_dynamic()?"enabled":"disabled") << endl;
    cout << "Nested parallelism is   "
         << ((omp_get_max_active_levels() > 1)?"enabled":"disabled")
         << " (" << omp_get_max_active_levels() << " levels)" << endl;
    cout << "Clock resolution is     " << omp_get_wtick() << " s" << endl;
    cout << "------------------------\n";
	
	// Test calculation
    printf ("OpenMP threads (%d): calculating...\n", n);
    double t = omp_get_wtime();
    long N = 1e8;
    long total = pc_static_1(N);
    t = omp_get_wtime() - t;
    printf ("Primes in an interval [1:%g]: %ld\n", (double)N, total);
    printf ("Calculated in %.2f s\n", t);	
}

