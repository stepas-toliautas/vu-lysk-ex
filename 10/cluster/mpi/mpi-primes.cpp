#include <cstdio>
#include "mpi.h"
#include "primes.h"

int main (int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int pid, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (pid == 0)
    {
        int version, subversion;
        MPI_Get_version(&version, &subversion);
        printf ("Running MPI %d.%d\n", version, subversion);
        
	char name[MPI_MAX_PROCESSOR_NAME] = "";
        int namelen; MPI_Get_processor_name(name, &namelen);
        printf ("Hello from process %d (of %d) on \"%s\"!\n",
            pid, p, name);
        
	printf ("MPI processes (%d): calculating...\n", p);
        fflush(stdout);
    }

    long N = 100*1000*1000;
    long Ni = N;                            // Last number to check
    long fi = (1+2*pid) | 1;                // First number to check
    long si = 2*p;                          // Skip even numbers

    double t = MPI_Wtime();
    long total = 0, primes = prime_count(Ni, fi, si);
    MPI_Reduce(&primes, &total, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    t = (MPI_Wtime() - t);

    if (pid == 0)
    {
        printf ("Found %8ld primes in [%g:%g]", total, (double)fi, (double)N);
        printf (" (in %.3f s)\n", t);
        fflush(stdout);
    }

    MPI_Finalize();
}
