#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
// #include "KVCache.h"
#include "threadpool.h"
#include "serveRequest.h"

static const size_t num_threads = 1;
static const size_t num_items   = 10;

// void worker(void *arg)
// {
//     // int *val = arg;
//     char *val = arg;
//     //int  old = *val;

//     //*val += 1000;
//     printf("tid=%ld, val=%s\n", pthread_self(), val);

//     // if (*val%2)
//     //     usleep(100000);
// }

void worker(char *arg, char **arg2)
{
    // int *val = arg;
    // char *val = arg;
    //int  old = *val;
    static int c =0;
    //*val += 1000;
    char *temp = (char *)malloc(30*sizeof(char));
    strcpy(temp, "new data");
    printf("tid=%ld\n", pthread_self());
    // strcpy(arg, "go to hell");
    // printf("tid=%ld, val=%s\n", pthread_self(), arg);
    temp[strlen(temp)] = '\0';
        *arg2 = temp;
    c++;
    //if (c%2)
        //usleep(100000);
    
}

int main(int argc, char **argv)
{
    tpool_t *tm;
    int     *vals;
    size_t   i;
    cacheInit();
    tm   = tpool_create(num_threads);
    vals = calloc(num_items, sizeof(*vals));
    char buffer[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><KVMessage type=\"putreq\"><Key>333</Key><Value>value111_111</Value></KVMessage>";
    char *tempBuffer;

    for (i=0; i<num_items; i++) {
        vals[i] = i;
        tpool_add_work(tm, serveRequest, buffer, &tempBuffer);

    }
    
    usleep(100);
    tpool_wait(tm);

    for (i=0; i<num_items; i++) {
        printf("%d from main function: %s\n",i, tempBuffer);//        printf("%d\n", vals[i]);
    }

    free(vals);
    tpool_destroy(tm);
    return 0;
}