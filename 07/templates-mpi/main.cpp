#include <cstdio>
#include "mpi.h"

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
    }
    char name[MPI_MAX_PROCESSOR_NAME] = "";
    int namelen; MPI_Get_processor_name(name, &namelen);
    printf ("Hello from process %d (of %d) on \"%s\"!\n",
            pid, p, name);
    MPI_Finalize();
}
