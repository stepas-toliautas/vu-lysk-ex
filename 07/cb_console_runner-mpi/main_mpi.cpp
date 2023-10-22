/*
 * This file is part of the Code::Blocks IDE and licensed under the GNU General Public License, version 3
 * http://www.gnu.org/licenses/gpl-3.0.html
 *
 * $Revision$
 * $Id$
 * $HeadURL$
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#ifdef __WXMSW__
    #include <windows.h>
    #include <conio.h>
    #define wait_key getch
#else
    #define wait_key getchar
#endif
#if defined(__unix__) || defined(__unix)
    #include <sys/wait.h>
#endif
#include <string.h>

#ifdef __MINGW32__
int _CRT_glob = 0;
#endif

// Check whether string has spaces
bool hasSpaces(const char* str)
{
    char last = 0;
    while (str && *str)
    {
        if ((*str == ' ' || *str == '\t') && last != '\\')
            return true;
        last = *str++;
    }
    return false;
}

// Enclose argument in quotes if it has spaces
char* strcatQuoted (char* line, const char* arg)
{
    bool sp = hasSpaces(arg);
    if (sp)
        strcat(line, "\"");
    strcat(line, arg);
    if (sp)
        strcat(line, "\"");
    strcat(line, " ");
    return line;
}

#ifdef MPI_ENABLED
// Define MPI options
const char* mpi_arg = "--mpi";
const char* arg_exe = "-exec";
const char* arg_opt = "-options";

// Set up MPI job
bool checkMPI(int argc, char** argv, char* cmdline)
{
    char mpi_exec[256] = "mpiexec";
    char mpi_opts[256] = "";
    bool mpi = false;

    for (int i = 1; i < argc; ++i)
    {
        // Skip non-MPI arguments
        if (strncmp(argv[i], mpi_arg, strlen(mpi_arg)) != 0)
            continue;
        int j = strlen(mpi_arg);
        // Copy name of MPI executable
        if (strncmp(&(argv[i])[j], arg_exe, strlen(arg_exe)) == 0)
            strcpy(mpi_exec, &(argv[i])[j + strlen(arg_exe) + 1]);
        // Copy MPI options
        else if (strncmp(&(argv[i])[j], arg_opt, strlen(arg_opt)) == 0)
            strcpy(mpi_opts, &(argv[i])[j + strlen(arg_opt) + 1]);
        mpi = true;
    }
    // Write MPI options to the command line
    if (mpi)
    {
        strcatQuoted(cmdline, mpi_exec);
        strcat(cmdline, mpi_opts);
        strcat(cmdline, " ");
    }
    return mpi;
}

// Check if argument belongs to MPI setup
inline
bool skipMPI(char* arg)
{
    return (strncmp(arg, mpi_arg, strlen(mpi_arg)) == 0);
}
#endif

int execute_command(char *cmdline)
{
#ifdef __WXMSW__
    //Windows's system() seems to not be able to handle parentheses in
    //the path, so we have to launch the program a different way.

    SetConsoleTitle(cmdline);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    // Start the child process.
    CreateProcess( NULL, TEXT(cmdline), NULL, NULL, FALSE, 0,
                   NULL, NULL, &si, &pi );

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Get the return value of the child process
    DWORD ret;
    GetExitCodeProcess( pi.hProcess, &ret );

    // Close process and thread handles.
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );

    return ret;
#else
    int ret = system(cmdline);
    if(WIFEXITED(ret))
    {
        return WEXITSTATUS(ret);
    }
    else
    {
        return -1;
    }
#endif
}

// Main function
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: cb_console_runner <filename> <args ...>\n");
        return 1;
    }

    // Count size of arguments
    size_t fullsize = 0;
    for (int i = 1; i < argc; ++i)
    {
        fullsize += strlen(argv[i]);
    }
    // Add some slack for spaces between args plus quotes around executable
    fullsize += argc + 32;

    char* cmdline = new char[fullsize];
    memset(cmdline, 0, fullsize);

#ifdef MPI_ENABLED
    // Set up MPI job
    bool mpi = checkMPI(argc, argv, cmdline);
#endif

    // Arguments enclosed in quotes to support filenames with spaces
    for (int i = 1; i < argc; ++i)
    {
#ifdef MPI_ENABLED
        if (mpi && skipMPI(argv[i])) continue;
#endif
        strcatQuoted(cmdline, argv[i]);
    }

    timeval tv;
    gettimeofday(&tv, NULL);
    double cl = tv.tv_sec + (double)tv.tv_usec / 1000000;

    int ret = execute_command(cmdline);

    gettimeofday(&tv, NULL);
    cl = (tv.tv_sec + (double)tv.tv_usec / 1000000) - cl;

    printf("\nProcess returned %d (0x%X)   execution time : %0.3f s", ret, ret, cl);
    printf
    (
        "\nPress "
#ifdef __WXMSW__
        "any key"
#else
        "ENTER"
#endif
        " to continue.\n"
    );

    wait_key();

    delete[] cmdline;
    return ret;
}
