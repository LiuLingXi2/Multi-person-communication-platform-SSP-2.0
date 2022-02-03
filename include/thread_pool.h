/*
 * @File Name: thread_pool.h
 * @Mail: 1.0
 * @Author: LH
 * @Created Time: Do not edit
 */

#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include "../common/common.h"

struct task_queue {
    int head, tail, size, total;
    void **data;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

void task_queue_init(struct task_queue *taskQueue, int size);
void task_queue_push(struct task_queue *taskQueue, void *data);
void* task_queue_pop(struct task_queue *taskQueue);
void task_queue_destroy(struct task_queue *taskQueue);

void* thread_run(void *arg);
#endif // !__THREAD_POOL_H__