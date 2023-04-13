#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

double accumulator = 0.0;

struct arguments
{
    int lb;
    int rb;
    int n;
};

double f(double x)
{
    return 1/(1+x*x);
}

struct par
{
    int a;
    int b;
    int n;
};

void *sum(void *p)
{
    double *r;
    struct par *p1 = (struct par *)p;
    int i;
    r = (double *)malloc(sizeof(double));
    *r = 0.0;
    for(i = p1->a; i<p1->b; i++)
        *r += f(i / (double)p1->n);
    free(p);
    return r;
}

int main()
{
    int n;
    int thread_count;
    
    printf("n=");
    scanf("%d", &n);
    printf("thread count=");
    scanf("%d", &thread_count);
    pthread_t threads[thread_count];
    struct par *p;
    void *response[thread_count];
    double r;
    
    float delta = n / thread_count;
    for (int i = 0; i < thread_count; i++)
    {
        int left_border = 1 + delta * i;
        int right_border = delta + 1 + delta * i;
        if (i == thread_count - 1)
        {
            right_border = right_border - 1;
        }

        p = (struct par *)malloc(sizeof(struct par));
        p->a = left_border;
        p->b = right_border;
        p->n = n;
        pthread_create(&threads[i], NULL, sum, p);
    }
    
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], &response[i]);
    }

    double acc = 0.0;

    for (int i = 0; i < thread_count; i++)
    {
        acc = acc + *(double *)response[i];
    }

    /*
    p = (struct par *) malloc(sizeof(struct par));
    p->a = 0.0;
    p->b = 1;
    p->e = n/2;
    p->h = 1.0/n;
    pthread_create(&w1, NULL, sum, p);
    p = (struct par *) malloc(sizeof(struct par));
    p->a = 0.0;
    p->b = n/2+1;
    p->e = n-1;
    p->h = 1.0/n;
    pthread_create(&w2, NULL, sum, p);
    */
 //   pthread_join(w1, &r1);
 //   pthread_join(w2, &r2);
    r = 2 * (f(0.0) + f(1.0) + 2 * acc) / n;
    printf("\nPI = %g\n", r);
    //free(r1);
    //free(r2);
    
    for (int i = 0; i < thread_count; i++)
    {
        free(response[i]);
    }
    return EXIT_SUCCESS;
}