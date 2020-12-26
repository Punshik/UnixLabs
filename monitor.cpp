#include <unistd.h>
#include <cstdio>
#include <pthread.h>

pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int ready = 0;

void *subscriber(void *par)
{
    while (true) {
        pthread_mutex_lock(&lock);

        while (ready == 0) {
            pthread_cond_wait(&cond1, &lock);
            printf("awoke \n");
        }

        ready = 0;
        printf("consumed \n");

       pthread_mutex_unlock(&lock);
    }
}


void *provider(void *par)
{
    while (true) {
        pthread_mutex_lock(&lock);

        if(ready == 1) {
            pthread_mutex_unlock(&lock);
            continue;
        }

        ready = 1;

        printf("provided \n");
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&lock);

        sleep (1);
    }
}

int main()
{
    pthread_t firstThread, secondThread;
    pthread_create(&firstThread, nullptr, provider, nullptr);
    pthread_create(&secondThread, nullptr, subscriber, nullptr);
    pthread_join(firstThread, nullptr);
    pthread_join(secondThread, nullptr);
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond1);
}


