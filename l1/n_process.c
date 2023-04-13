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
            // printf("\nlb: %i, rb: %i, sum: %lf\n", left_border, right_border, s);
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
            close(pipes[i][0]);
    }
    printf("PI: %f\n", 2 * (f(0.0) + f(1.0) + 2 * accumulator) / n);
            
}

int main()
{ 
    int n, process_count;
    printf("n=");
    scanf("%d", &n);
    printf("thread count=");
    scanf("%d", &process_count);
 
    p(process_count, n);
}