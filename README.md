# Project 1 README
## SUBMISSION BY:
    + Cassie Peters ( 63685507 )
    + Fareeha Hayat ( 20489167 )
    + Amrita Sidhu ( 85822294 )



## OVERVIEW OF PROGRAM LOGIC:

Parallel and Sequential runs of processes are handled nearly in the same way with one minor caveat; the waitpid() function.

## ++ BEFORE WE CHECK THE USER INPUT ++
    1.  We take the time the program begins at and save it to the variable 'start'


    ++ SEQUENTIAL RUNS ++

    1.  An array is initialized to store all child process ids.
    2.  Then we fork only the child processes to ensure that we only get n-processes (if we forked both the parent and child we would get 2^n+1 processes which != the count input by the user).
    *   [ All processes are running in parallel by default after forking ]
    3.  Each child process then moves forward [ (pid==0) ]
    4.  We check the time that has elapsed ()
    5.  If the time elapsed is less than the timeout set by the user or the timeout == 0 (no time restriction) and when the process id (PID) is equal to zero we then execute the command and finally kill the process
    6.  Otherwise the time elapsed is greater than the timeout set by the user and the process(es) is/are terminated without executing







    ++ PARALLEL RUNS ++

    1.  Then we fork only the child processes to ensure that we only get n-processes (if we forked both the parent and child we would get 2^n+1 processes which != the count input by the user).
                    *   [ All processes are running in parallel by default after forking ]
    2.  We then have the parent wait for the child to execute and terminate before each parent continues through the code. This ensures each process runs sequentially. [ waitpid(pid) ]
    3.  Each child process then moves forward [ (pid==0) ]
    4.  We check the time that has elapsed ()
    5.  If the time elapsed is less than the timeout set by the user or the timeout == 0 (no time restriction) and when the process id (PID) is equal to zero we then execute the command and finally kill the process
    6.  Otherwise the time elapsed is greater than the timeout set by the user and the process(es) is/are terminated without executing
    7.  After each child process is killed the parent of the killed child then resumes moving through the code
    This happens until the finally parent is reached and then executed







## SOURCES OF HELP

1. https://www.programiz.com/c-programming/c-arrays
    +++ Syntactical help for the construction and calling of arrays in C
2. https://www.cplusplus.com/reference/ctime/difftime/
    ++++ for help with finding an easy way to determine the change in time from the
         start of the program to when a process begins [difftime()]
3. https://www.tutorialspoint.com/c_standard_library/c_function_time.htm
    +++ help for understanding the time_c data type.
        We needed to know if we had to cast it to a double to compare it with the timeout

