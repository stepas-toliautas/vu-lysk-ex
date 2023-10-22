#include <cstdio>
#include <ctime>
#include <windows.h>

const char* worker = "single-cmd.exe";
void winproc_prime_count (long N);

const int WORKERS = 3;

int main ()
{
    long N = 1e8;
    printf ("WINAPI processes (%d x %s): calculating...\n", WORKERS, worker);
    long t = clock();
    winproc_prime_count(N);
    t = clock() - t;
    printf ("Calculated in %.2f s\n", ((float)t)/CLOCKS_PER_SEC);
}

void winproc_prime_count (long N)
{
    STARTUPINFO         st[WORKERS];
    PROCESS_INFORMATION pr[WORKERS];
    for (int i=0; i<WORKERS; ++i)
    {
        /* Prepare worker arguments */
        long Ni = (i+1)*N/WORKERS;              // Last number to check
        long fi = i*N/WORKERS;                  // First number to check
        if (fi%2==0) fi += 1;                   // Start from odd number
        long si = 2;                            // Skip even numbers
        /* Prepare execution command */
        char command[256];
        sprintf(command, "%s %ld %ld %ld", worker, Ni, fi, si);
        /* Create process and execute worker */
        ZeroMemory(&st[i], sizeof(st[i]));
        st[i].cb = sizeof (st[i]) ;
        ZeroMemory(&pr[i], sizeof(pr[i]));
    /*int rc =*/ CreateProcess(NULL,            // Use command-line program
                               command,         // Command to run
                               NULL,            // Don't inherit process handle
                               NULL,            // Don't inherit thread handle
                               FALSE,           // Disable handle inheritance
                               0,               // No creation flags
                               NULL,            // Use parent's environment
                               NULL,            // Use parent's directory
                               &st[i],
                               &pr[i]);
    }
    /* Wait for all processes to end */
    for (int i=0; i<WORKERS; ++i)
    {
        WaitForSingleObject(pr[i].hProcess, INFINITE);
        CloseHandle(pr[i].hThread);
        CloseHandle(pr[i].hProcess);
    }
}
