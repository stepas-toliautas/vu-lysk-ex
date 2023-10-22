#include <iostream>
#include <cmath>
#include <omp.h>
using namespace std;

const int MAX_WORKERS = 8;

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
         << (omp_get_nested()?"enabled":"disabled") << endl;
    cout << "Clock resolution is     " << omp_get_wtick() << " s" << endl;
    cout << "------------------------\n";
}
