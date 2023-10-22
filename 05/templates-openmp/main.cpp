#include <iostream>
#include <omp.h>
using namespace std;

int main ()
{
    #ifdef _OPENMP
    cout << "OpenMP standard date:   " << _OPENMP << "\n";
    #endif
    omp_set_num_threads(7);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        //#pragma omp critical
        cout << tid << ": Hello world!\n";
    }
}
