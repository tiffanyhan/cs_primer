#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
        printf("error: capacity reached\n");
        return;
    }
    jq->buffer[jq->wp] = item;
    printf("%d -> [%d]\n", item, jq->wp);
    jq->wp = (jq->wp + 1) % jq->capacity;
    jq->size++;
}

job JQ_get(struct JobQueue *jq) {
    // if (jq->size <= 0) {
    //     return void;
    // }
    job j = jq->buffer[jq->rp];
    printf("     [%d] -> %d\n", jq->rp, j);
    jq->rp = (jq->rp + 1) % jq->capacity;
    jq->size--;
    return j;
}

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

    return 0;
}
