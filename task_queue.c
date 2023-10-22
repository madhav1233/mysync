// task_queue.c

#include "mysync.h"
#include "task_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

extern int num_input_dirs;
// Create a new, empty task queue
TaskQueue* taskqueue_new(void) {
    TaskQueue *queue = (TaskQueue *)malloc(sizeof(TaskQueue));
    if (!queue) {
        perror("Failed to allocate memory for task queue");
        exit(EXIT_FAILURE);
    }
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}

// Enqueue a new item to the task queue
void taskqueue_enqueue(TaskQueue *queue, bool change_required, char *filepath, DirectoryStatus *dirs, int num_dirs) {
    TaskQueueItem *newItem = (TaskQueueItem *)malloc(sizeof(TaskQueueItem));
    if (!newItem) {
        perror("Failed to allocate memory for task queue item");
        exit(EXIT_FAILURE);
    }
    newItem->change_required = change_required;
    newItem->filepath = strdup(filepath);
    if (!newItem->filepath) {
        free(newItem);
        perror("Failed to allocate memory for filepath");
        exit(EXIT_FAILURE);
    }
    newItem->directories = (DirectoryStatus *)malloc(sizeof(DirectoryStatus) * num_dirs);
    if (!newItem->directories) {
        free(newItem->filepath);
        free(newItem);
        perror("Failed to allocate memory for directories");
        exit(EXIT_FAILURE);
    }
    memcpy(newItem->directories, dirs, sizeof(DirectoryStatus) * num_dirs);
    newItem->next = NULL;

    if (queue->tail) {
        queue->tail->next = newItem;
        queue->tail = newItem;
    } else {
        queue->head = newItem;
        queue->tail = newItem;
    }
}

// Dequeue an item from the task queue
TaskQueueItem* taskqueue_dequeue(TaskQueue *queue) {
    if (!queue->head) {
        return NULL;
    }
    TaskQueueItem *dequeuedItem = queue->head;
    queue->head = queue->head->next;
    if (!queue->head) {
        queue->tail = NULL;
    }
    dequeuedItem->next = NULL;
    return dequeuedItem;
}

// Find an item in the task queue based on filepath
bool taskqueue_find(TaskQueue *queue, char *filepath) {
    TaskQueueItem *current = queue->head;
    while (current) {
        if (strcmp(current->filepath, filepath) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Print the task queue
void taskqueue_print(TaskQueue *queue) {
    if (!queue || !queue->head) {
        printf("Queue is empty.\n");
        return;
    }

    TaskQueueItem *current = queue->head;
    while (current) {
        printf("\nFilepath: %s\n", current->filepath);
        for (int i = 0; i < num_input_dirs; i++) {
            printf("Directory: %d, Modification Time: %ld, Is Newest: %d\n",
                   current->directories[i].input_dir, current->directories[i].modification_time, current->directories[i].is_newest);
        }
        current = current->next;
    }
}
