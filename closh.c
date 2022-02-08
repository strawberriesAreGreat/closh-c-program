/* closh.c - COSC 315, Winter 2020

SUBMISSION BY:
    + Cassie Peters ( 63685507 )
    + Fareeha Hayat ( 20489167 )
    + Amrita Sidhu  ( 85822294 )

SOURCES OF HELP

1. https://www.programiz.com/c-programming/c-arrays
    +++ Syntactical help for the construction and calling of arrays in C
2. https://www.cplusplus.com/reference/ctime/difftime/
    ++++ for help with finding an easy way to determine the change in time from the
         start of the program to when a process begins [difftime()]
3. https://www.tutorialspoint.com/c_standard_library/c_function_time.htm
    +++ help for understanding the time_c data type.
        We needed to know if we had to cast it to a double to compare it with the timeout

*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// needed for the clock function and clocks per second value and boolean data type for debugging
#include <time.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

// tokenize the command string into arguments - do not modify
void readCmdTokens(char *cmd, char **cmdTokens)
{
    cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
    int i = 0;
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens))
    {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// read one character of input, then discard up to the newline - do not modify
char readChar()
{
    char c = getchar();
    while (getchar() != '\n')
        ;
    return c;
}

// main method - program entry point
int main()
{
    char cmd[81];        // array of chars (a string)
    char *cmdTokens[20]; // array of strings
    int count;           // number of times to execute command
    int parallel;        // whether to run in parallel or sequentially
    int timeout;         // max seconds to run set of commands (parallel) or each command (sequentially)

    while (TRUE)
    { // main shell input loop

        // begin parsing code - do not modify
        printf("closh> ");
        fgets(cmd, sizeof(cmd), stdin);
        if (cmd[0] == '\n')
            continue;
        readCmdTokens(cmd, cmdTokens);
        do
        {
            printf("  count> ");
            count = readChar() - '0';
        } while (count <= 0 || count > 9);

        printf("  [p]arallel or [s]equential> ");
        parallel = (readChar() == 'p') ? TRUE : FALSE;
        do
        {
            printf("  timeout> ");
            timeout = readChar() - '0';
        } while (timeout < 0 || timeout > 9);
        // end parsing code

        /*
            OUR CODE BEGINS BELOW:

                ───▄▄▄              ───▄▄▄               ───▄▄▄                 ───▄▄▄
                ─▄▀░▄░▀▄            ─▄▀░▄░▀▄             ─▄▀░▄░▀▄               ─▄▀░▄░▀▄
                ─█░█▄▀░█            ─█░█▄▀░█             ─█░█▄▀░█               ─█░█▄▀░█
                ─█░▀▄▄▀█▄█▄▀        ─█░▀▄▄▀█▄█▄▀         ─█░▀▄▄▀█▄█▄▀           ─█░▀▄▄▀█▄█▄▀
                ▄▄█▄▄▄▄███▀         ▄▄█▄▄▄▄███▀          ▄▄█▄▄▄▄███▀             ▄▄█▄▄▄▄███▀


            OVERVIEW OF PROGRAM LOGIC:

                Parallel and Seqential runs of processes are handled nearly in the same way with with one
                minor caveat; the waitpid() funtion.


            ++ BEFORE WE CHECK THE USER INPUT ++
                1.  We take the time the program begins at and save it to the variable 'start'


            ++ SEQUENTIAL RUNS ++

                1.  An array is initialized to store all child process ids.
                2.  Then we fork only the child processes to ensure that we
                    only get n-processes (if we forked both the parent and child we would get 2^n+1 processes which != the count input
                    by the user).
                *   [ All processes are running in parallel by default after forking ]
                3.  Each child process then moves forward [ (pid==0) ]
                4.  We check the time that has elapsed ()
                5.  If the time elapsed is less than the timeout set by the user or the timout == 0 (no time restriction)
                    and when the process id (PID) is equal to zero we then execute the command and finally kill the process
                6.  Otherwise the time elapsed is greater than the timeout set by the user and the process(es) is/are terminated
                    without executing


            ++ PARALLEL RUNS ++

                1.  Then we fork only the child processes to ensure that we
                    only get n-processes (if we forked both the parent and child we would get 2^n+1 processes which != the count input
                    by the user).
                *   [ All processes are running in parallel by default after forking ]
                2.  We then have the parent wait for the child to execute and terminate before each parent continues through
                    the code. This ensures each process runs sequentially. [ waitpid(pid) ]
                3.  Each child process then moves forward [ (pid==0) ]
                4.  We check the time that has elapsed ()
                5.  If the time elapsed is less than the timeout set by the user or the timout == 0 (no time restriction)
                    and when the process id (PID) is equal to zero we then execute the command and finally kill the process
                6.  Otherwise the time elapsed is greater than the timeout set by the user and the process(es) is/are terminated
                    without executing
                7.  After each child process is killed the parent of the killed child then resumes moving through the code
                    This happens until the finally parent is reached and then executed


        */

        // our program creates count+1 processes but only count number of executes as per the instructions

        int i, pid;         // i is the counter to determine the number of children to create, pid is the process id
        double timePassed;  // the change in time from the start of the program until a process begins
        time_t start, stop; //

        // starting the timer
        start = time(NULL);

        // checking if the processes are to be run sequentially or in parallel
        if (parallel < 1)
        {
            // running in sequentially
            for (i = 0; i < count; i++)
            {
                // forking the process and saving the child id to 'pid' (2^n + 1 n)
                pid = fork();
                // when the process has a pid > 0 it is a parent process and is forced to wait for the children to terminate before continuing
                if (pid > 0)
                {
                    getpid();
                    waitpid(pid);
                }
                // when the process is a child
                if (pid == 0)
                {
                    sleep(2); // just for proving the code works
                    stop = time(NULL);
                    timePassed = difftime(stop, start);
                    /*just for debugging the code
                    sleep(1);
                        bool run = (timePassed <= timeout);
                            printf("time: %f\n",timePassed);
                                printf("timeout: %d\n",timeout);
                                    printf("past: %ld\n",start);
                                        printf("stop: %ld\n",stop);
                                            printf("run: %d\n",run);
                    */
                    if ((timePassed < timeout) || (timeout == 0))
                    {
                        // execute command
                        printf("Executing Process Id: %d\n", getpid());
                        execvp(cmdTokens[0], cmdTokens);

                    }
                    else
                    {
                        // otherwise too much time has ellapsed. Kill the process.
                        printf("Failed to execute in time specified\n");
                        kill(getpid(), SIGINT);
                    }
                }
            }
        }
        else
        {
            // running in parallel
            // array initialization. Values set to 1 for debugging's sake
            int idArray[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

            for (i = 0; i < count; i++)
            {

                pid = fork();
                if (pid > 0)
                {
                    // saving the child process id to the array
                    idArray[i] = pid;
                }

                if (pid == 0)
                {
                   sleep(2); // just for proving the code works
                    stop = time(NULL);
                    timePassed = difftime(stop, start);

                    /* just for debugging the code
                        sleep(1);
                        bool run = (timePassed <= timeout);
                            printf("time: %f\n",timePassed);
                                printf("timeout: %d\n",timeout);
                                    printf("start: %ld\n",start);
                                        printf("stop: %ld\n",stop);
                                            printf("run: %d\n",run);
                    */

                    if ((timePassed < timeout) || (timeout == 0))
                    {
                  
                        printf("Executing Process Id: %d\n", getpid());
                        execvp(cmdTokens[0], cmdTokens);

                    }
                    else
                    {
                        printf("Failed to execute in time specified\n");
                        kill(getpid(), SIGKILL);
                    }
                }
            }
            printf("\nArray Of Child Processes: [");
            for (int x = 0; x < 9; x++)
            {
                printf(" %d,", idArray[x]);
            }
            printf("] ( a value of 1 means the array value is null) ");
        }

        // only executes the following if any process has failed (process id = -1 )
        if (getpid() < 0)
            printf("Can't execute %s\n", cmdTokens[0]); // only reached if running the program failed
        exit(1);
    }
}
