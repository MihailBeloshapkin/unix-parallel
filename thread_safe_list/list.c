#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct list
{
	int data;
    struct list *next;
} List;

typedef struct thread_safe_list
{
    List* list;
    pthread_mutex_t lock;
} TSList;

TSList *Create()
{
    TSList *l = (TSList *)malloc(sizeof(TSList));
    l->list = NULL;
    pthread_mutex_init(&l->lock, NULL);
    return l;
}

// Add to list front.
int AddSafe(TSList **l, int new_data)
{
    if (*l == NULL)
    {
        return -1;
    //   *l = (TSList *)malloc(sizeof(TSList));
    //   (*l)->list = NULL;
    //   pthread_mutex_init(&(*l)->lock, NULL); 
    }

    pthread_mutex_lock(&(*l)->lock);

    List* list = (*l)->list;
    List *new_element;
    if ((new_element = (List *)malloc(sizeof(List))) == NULL)
    {
        return -1;
    }

    new_element->data = new_data;
    new_element->next = list;
    (*l)->list = new_element;
    
    pthread_mutex_unlock(&(*l)->lock);
    return 0;
}

// Delete head element.
int DeleteSafe(TSList **l)
{
    if (*l == NULL)
    {
        return -1;
    }
    pthread_mutex_lock(&(*l)->lock);

    List* head_element = (*l)->list;
    (*l)->list = (*l)->list->next;
    free(head_element);

    pthread_mutex_unlock(&(*l)->lock);
    return 0;
}

int Clear(TSList **l)
{
    return 0;
}

void PrintList(TSList* l)
{
    if (l == NULL)
    {
        return;
    }

    pthread_mutex_lock(&l->lock);
    List* content = l->list;
    while (content != NULL)
    {
        printf("%d, ", content->data);
        content = content->next;
    }

    pthread_mutex_unlock(&l->lock);
}

int ListSize(TSList *l)
{
    if (l == NULL)
    {
        return -1;
    }

    pthread_mutex_lock(&l->lock);
    List* content = l->list;
    int size = 0;
    while (content != NULL)
    {
        size++;
        content = content->next;
    }

    pthread_mutex_unlock(&l->lock);
    return size;
}

void *sum(void *p)
{
    double *r;
    TSList **l = (TSList **)p;
    AddSafe(l, 30);
    return NULL;
}

int test()
{
    const int thread_count = 1000;
    pthread_t threads[thread_count];
    TSList* list = Create();
    void* r;

    for (int i = 0; i < thread_count; i++)
    {
        pthread_create(&threads[i], NULL, sum, &list);
    }
    
    for (int i = 0; i < thread_count; i++)
    {
        pthread_join(threads[i], r);
    }
    PrintList(list);
    printf("List size: %d", ListSize(list));
}

int main()
{
    test();
    /*
    TSList* list = Create();
    AddSafe(&list, 30);
    AddSafe(&list, 31);
    AddSafe(&list, 32);
    DeleteSafe(&list);
    //if (list == NULL)
    //{
    //    printf("NULL");
    //}
    printf("Head: %d\n", list->list->data);
    PrintList(list);
    */
    return 0;
}