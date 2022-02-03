#include "../common/common.h"

void task_queue_init(struct task_queue *taskQueue, int size) {
    taskQueue->size = size;
    taskQueue->total = taskQueue->head = taskQueue->tail = 0;
    taskQueue->data = calloc(size, sizeof(void *));

    pthread_mutex_init(&taskQueue->mutex, NULL);
    pthread_cond_init(&taskQueue->cond, NULL);

    return ;
}

void task_queue_push(struct task_queue *taskQueue, void *data) {
    pthread_mutex_lock(&taskQueue->mutex);
    if (taskQueue->total == taskQueue->size) {
        /* Task queue is full */
        DBG(YELLOW"<push> taskQueue is full \n"NONE);
        pthread_mutex_unlock(&taskQueue->mutex);
        return ;
    }
    taskQueue->data[taskQueue->tail] = data;
    DBG(PINK"<push> push to %dth task.\n"NONE, taskQueue->tail);
    taskQueue->total ++;
    if (++ taskQueue->tail == taskQueue->size) {
        /* The tail pointer is already at the end of the queue, start from the beginning */
        DBG(PINK"<push> tail begins with 0. \n"NONE);
        taskQueue->tail = 0;
    } 
    pthread_cond_signal(&taskQueue->cond);
    pthread_mutex_unlock(&taskQueue->mutex);

    return ;
}

void* task_queue_pop(struct task_queue *taskQueue) {
    pthread_mutex_lock(&taskQueue->mutex);
    while (taskQueue->total == 0) { /* swarm effect */
        pthread_cond_wait(&taskQueue->cond, &taskQueue->mutex);
    }
    void *data = taskQueue->data[taskQueue->head];
    DBG(PINK"<pop> pop data from %dth task.\n"NONE, taskQueue->head);
    taskQueue->total --;
    if (++ taskQueue->head == taskQueue->size) {
        DBG(BLUE"<pop> head begins with 0. \n"NONE);
        taskQueue->head = 0;
    }
    pthread_mutex_unlock(&taskQueue->mutex);
    return data;
}

void task_queue_destroy(struct task_queue *taskQueue) {
    pthread_mutex_destroy(&taskQueue->mutex);
    pthread_cond_destroy(&taskQueue->cond);
    free(taskQueue->data);
    taskQueue->data = NULL;
    free(taskQueue);
    taskQueue = NULL;
}

void* thread_run(void *arg) {
    pthread_detach(pthread_self());
    struct task_queue *taskQueue = (struct task_queue *)arg;
    while (1) {
        void *data = task_queue_pop(taskQueue);
        DBG(GREEN"<thread> got a task. \n"NONE);
        printf("%s", data);
    }
}