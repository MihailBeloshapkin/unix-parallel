#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct queue
{
	int data;
    struct queue *next;
} Queue;

typedef struct thread_safe_queue
{
    Queue* head;
    Queue* tail;
    int size;
    pthread_mutex_t head_mutex;
    pthread_mutex_t tail_mutex;
} TSQueue;

TSQueue *Create()
{
    TSQueue *q = (TSQueue *)malloc(sizeof(TSQueue));
    q->head = NULL;
    q->tail = NULL;
    q->size = NULL;
    pthread_mutex_init(&q->head_mutex, NULL);
    pthread_mutex_init(&q->tail_mutex, NULL);
    // pthread_mutex_lock(&(q->tail_mutex));
    return q;
}

// Add to back.
int Enqueue(TSQueue **q, int new_data)
{
    if (*q == NULL)
    {
        return -1;
    }
    
    pthread_mutex_lock(&(*q)->head_mutex);
    Queue *new_element;
    
    if ((new_element = (Queue *)malloc(sizeof(Queue))) == NULL)
    {
        return -1;
    }

    new_element->data = new_data;   
    Queue* tail = (*q)->tail;
    if (tail == NULL)
    {
        (*q)->head = new_element;
        (*q)->tail = new_element;
        pthread_mutex_unlock(&(*q)->head_mutex);    
        return 0;
    }

    if ((*q)->head->next == NULL)
    {
        (*q)->head->next = new_element;
        (*q)->tail = new_element;
        pthread_mutex_unlock(&(*q)->head_mutex);
        return 0;
    }

    (*q)->tail->next = new_element;
    (*q)->tail = new_element;
    pthread_mutex_unlock(&(*q)->head_mutex);
    // tail = new_element;
    return 0;
}

// Delete from head.
int Dequeue(TSQueue **q)
{
    if (*q == NULL)
    {
        return -1;
    }
    pthread_mutex_lock(&(*q)->head_mutex);

    if ((*q)->head == NULL)
    {

        pthread_mutex_unlock(&(*q)->head_mutex);
        return -2;
    }

    Queue *head = (*q)->head;
    (*q)->head = (*q)->head->next;
    free(head);
    pthread_mutex_unlock(&(*q)->head_mutex);
    return 0;
}

void PrintQueue(TSQueue *q)
{
    if (q == NULL)
    {
        return;
    }

    Queue *content = q->head;

    while (content != NULL)
    {
        printf("%d, ", content->data);        
        content = content->next;
    }
}

/*
TQueue *Create()
{
    TSQueue *l = (TSQueue *)malloc(sizeof(TSQueue));
    l->list = NULL;
//    pthread_mutex_init(&l->lock, NULL);
    return l;
}

// Add to list front.
int Enqueue(TSQueue **l, int new_data)
{
    if (*l == NULL)
    {
        return -1;
    //   *l = (TSList *)malloc(sizeof(TSList));
    //   (*l)->list = NULL;
    //   pthread_mutex_init(&(*l)->lock, NULL); 
    }

    // pthread_mutex_lock(&(*l)->lock);

    List* list = (*l)->list;
    List *new_element;
    if ((new_element = (List *)malloc(sizeof(List))) == NULL)
    {
        return -1;
    }

    new_element->data = new_data;
    new_element->next = list;
    (*l)->list = new_element;
    
    // pthread_mutex_unlock(&(*l)->lock);
    return 0;
}*/

int main()
{
    TSQueue* q = Create();
    Enqueue(&q, 30);
    Enqueue(&q, 40);
    Enqueue(&q, 50);
    PrintQueue(q);
    printf("\n");
    Dequeue(&q);
    PrintQueue(q);
    return 0;
}