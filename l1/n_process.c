#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

double f(double x)
{
    return 1/(1+x*x);
}

double sum(int a, int b, int n)
{
    double r = 0;
    for (int i = a; i<b; i++)
        r += f(i/(double)n);
    
    return r;
}

void p(const int pc, int n)
{
    int pipes[pc][2];
    double accumulator = 0.0;
    pid_t new_pid;

    // RUN CALCS
    for (int i = 0; i < pc; i++)
    {
        pipe(pipes[i]);
        new_pid = fork();
        
        if (new_pid != (pid_t)0) // parent process
        {
        }
        else // child process
        {
            double s;
            close(pipes[i][0]);
            s = (double)1;
            
            float delta = n / pc;
            int left_border = 1 + delta * i;
            int right_border = delta + 1 + delta * i;
            if (i == pc - 1)
            {
                right_border = right_border - 1;
            }
            s = sum(left_border, right_border, n);
            printf("\nlb: %i, rb: %i, sum: %lf\n", left_border, right_border, s);
            // s = (double)i;
            write(pipes[i][1], &s, sizeof(double));
            close(pipes[i][1]);
            return;
        }
    }
    
    // read results
    for (int i = 0; i < pc; i++)
    {
            close(pipes[i][1]);
            double new_sum;
            read(pipes[i][0], &new_sum, sizeof(double));
            accumulator += new_sum;
            printf("New sum: %lf; acc = %lf\n", new_sum, accumulator);
            printf("PI: %g\n", 2 * (f(0.0) + f(1.0) + 2 * accumulator) / n);
            close(pipes[i][0]);
    }
    printf("\nResult: %lf", accumulator);
}

void many_processes(int process_count, int n)
{
    double accumulator = 0.0;
    for (int i = 0; i < 3; i++)
    {
        int new_fds[2];
        pid_t new_pid;
        pipe(new_fds);
        new_pid = fork();
        if (new_pid != (pid_t) 0) 
        {
            close(new_fds[1]);
            double new_sum = 0;
            read(new_fds[0], &new_sum, sizeof(double));
            accumulator += new_sum;
            printf("New sum: %lf; acc = %lf\n", new_sum, accumulator);
            close(new_fds[0]);
        }
        else // child process
        {

            double s;
            close(new_fds[0]);
            // s = sum(i / process_count, (i + 1) / process_count, n);
            s = (double)1;
            printf("iteration: %i, sum: %lf\n", i, s);
            write(new_fds[1], &s, sizeof(double));
            close(new_fds[1]);
            return;
        }
    }    
    printf("Result: %lf", accumulator);
}

int main()
{ 
    p(10, 100);
}