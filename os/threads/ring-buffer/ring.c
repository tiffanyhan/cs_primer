#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t rm, wm;

typedef int job;
typedef struct JobQueue {
    job *buffer;
    int capacity;
    int size;
    int rp;
    int wp;
} JobQueue;

JobQueue* JQ_init(int capacity) {
    JobQueue *jq;
    jq = (JobQueue*)malloc(sizeof(JobQueue));
    jq->buffer = malloc(sizeof(job) * capacity);
    jq->capacity = capacity;
    jq->size = 0;
    jq->rp = 0;
    jq->wp = 0; // possibly redundant with jq->size
    return jq;
}

void JQ_free(struct JobQueue *jq) {
    free(jq->buffer);
    free(jq);
}

void JQ_add(struct JobQueue *jq, job item) {
    if (jq->size == jq->capacity) {
        printf("add error: capacity reached\n");
        return;
    }
    jq->buffer[jq->wp] = item;
    printf("%d -> [%d]\n", item, jq->wp);
    jq->wp = (jq->wp + 1) % jq->capacity;
    jq->size++;
}

job JQ_get(struct JobQueue *jq) {
    pthread_mutex_lock(&rm);
    if (jq->size <= 0) {
        printf("get error: no items to get\n");
        return -1;
    }
    job j = jq->buffer[jq->rp];
    printf("     [%d] -> %d\n", jq->rp, j);
    jq->rp = (jq->rp + 1) % jq->capacity;
    jq->size--;
    pthread_mutex_unlock(&rm);
    return j;
}

void* producer(void *arg) {
    JobQueue *jq = (JobQueue*)arg;
    int i = 0;
    while (1) {
        JQ_add(jq, i++ % 100);
    }
};

void* consumer(void *arg) {
    JobQueue *jq = (JobQueue*)arg;
    int i = 0;
    while (1) {
        JQ_get(jq);
    }
};

int main() {
    printf("Starting basic test\n");
    struct JobQueue *jq = JQ_init(8);
    int i;
    for (i = 0; i < 5; i++)
        JQ_add(jq, i);
    for (i = 0; i < 5; i++)
        JQ_get(jq);
    for (i = 0; i < 5; i++)
        JQ_add(jq, i);
    for (i = 0; i < 5; i++)
        JQ_get(jq);
    JQ_free(jq);

    printf("Started threaded tests\n");

    jq = JQ_init(4);

    pthread_mutex_init(&wm, NULL);
    pthread_mutex_init(&rm, NULL);

    pthread_t p1, p2;
    pthread_create(&p1, NULL, producer, jq);
    pthread_create(&p2, NULL, consumer, jq);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    return 0;
}
