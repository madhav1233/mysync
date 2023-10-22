// task_queue.h

#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct {
    int input_dir;
    time_t modification_time;
    bool is_newest;
} DirectoryStatus;

typedef struct TaskQueueItem {
    bool change_required;
    char *filepath;
    DirectoryStatus *directories;
    struct TaskQueueItem *next;
} TaskQueueItem;

typedef struct {
    TaskQueueItem *head;
    TaskQueueItem *tail;
} TaskQueue;

TaskQueue* taskqueue_new(void);
void taskqueue_enqueue(TaskQueue *queue, bool change_required, char *filepath, DirectoryStatus *dirs, int num_dirs);
TaskQueueItem* taskqueue_dequeue(TaskQueue *queue);
bool taskqueue_find(TaskQueue *queue, char *filepath);
void taskqueue_print(TaskQueue *queue);

#endif // TASKQUEUE_H
