#include <iostream>
#include <cmath>
#include <omp.h>
using namespace std;

const int MAX_WORKERS = 8;

// Time-consuming routine
double sideJob(int n)
{
    double res = 0;
    for (long i=0; i<(1e8)/(n+1); i++)
    {
        res += sqrt(sqrt(n*i));
    }
    return res;
}

// Variables that will be assigned different scope
int n_persistent    = 42, // private for each thread, persisting between sections
    n_private       = 42, // private for each thread, local for a current section
    n_firstpriv     = 42, // private, initialized from original
    n_lastpriv      = 42, // private, value from last iteration written back to original
    n_shared        = 42; // shared and overwritten

int main ()
{
    // Ask user for thread count
    cout << "How many new threads you'd like to run?\n";
    int n; cin >> n;
    n = (n<1)?1:((n>MAX_WORKERS)?MAX_WORKERS:n);

    // Number of threads to execute
    omp_set_num_threads(n);

    int tid, i;

    // n_persistent is declared thread-private - each thread will use its own value
    // and remember it between different parallel sections
    #pragma omp threadprivate (n_persistent)

    // First serial section - all values are 42
    cout << "T0: Initial values: ";
    cout << n_persistent << " " << n_private << " "
         << n_firstpriv << " " << n_shared << "\n\n";

    // First parallel section
    #pragma omp parallel \
    private (n_private, tid) \
    firstprivate (n_firstpriv) \
    shared (n_shared)
    {
        tid = omp_get_thread_num();
        #pragma omp critical
        {
            cout << "T" << tid << ": values at section start: ";
            cout << n_persistent << " "     // ?
                 << n_private << " "        // ?
                 << n_firstpriv << " "      // ?
                 << n_shared << "\n";       // ?
        }
        sideJob(tid);
        // Data manipulation - each thread will do that
        n_persistent    += tid+1;
        n_private       += tid+1;
        n_firstpriv     += tid+1;
        n_shared        += tid+1;
    }

    // Second serial section
    cout << "\nT0: values after 1st section: ";
    cout << n_persistent << " " << n_private << " "
         << n_firstpriv << " " << n_shared << "\n\n";

    // Second parallel section
    #pragma omp parallel \
    private (n_private, tid) \
    firstprivate (n_firstpriv) \
    shared (n_shared)
    {
        tid = omp_get_thread_num();
        #pragma omp critical
        {
            cout << "T" << tid << ": values at section start: ";
            cout << n_persistent << " "     // ?
                 << n_private << " "        // ?
                 << n_firstpriv << " "      // ?
                 << n_shared << "\n";       // ?
        }
        sideJob(tid);
        // Data manipulation - each thread will do that
        n_persistent    += tid+1;
        n_private       += tid+1;
        n_firstpriv     += tid+1;
        n_shared        += tid+1;
    }

    // Third serial section
    cout << "\nT0: values after 2nd section: ";
    cout << n_persistent << " " << n_private << " "
         << n_firstpriv << " " << n_shared << "\n\n";

    // Special parallel section for n_lastpriv
    // Threads will split iterations among themselves
    #pragma omp parallel for private (i, tid) \
    lastprivate (n_lastpriv) schedule (static, 3)
    for (i=0; i<4*n; i++)
    {
        tid = omp_get_thread_num();
        n_lastpriv = tid+1;
    }
    // Output for n_lastpriv
    cout << "T0: n_lastpriv is " << n_lastpriv << endl; // ?
    cout << "Done\n";
}
