#include "file_ops.h"
#include "task_queue.h"
#include "cmd_parser.h"
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>

extern HASHTABLE *hashtable;
extern int num_input_dirs;
extern Flags flags;
TaskQueue *tasks;

void print_task_details(TaskQueueItem *item, int num_dirs) {
    printf("Filepath: %s\n", item->filepath);
    printf("Change Required: %d\n", item->change_required);
    for (int i = 0; i < num_dirs; i++) {
        printf("Directory %d: Exists: %d, Modification Time: %ld, Is Newest: %d\n",
               i, item->directories[i].input_dir, item->directories[i].modification_time, item->directories[i].is_newest);
    }
}

void traverse_hashtable(HASHTABLE *hashtable) {
    bool change_required = false;
    int num_old_files = 0;
    tasks = taskqueue_new();
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        change_required = false;
        num_old_files = 0;
        LIST *current = hashtable[i];

        if (!current) continue; // Skip if the current list is empty

        // Allocate memory for directory_status array
        DirectoryStatus *directory_status = (DirectoryStatus*) malloc(num_input_dirs * sizeof(DirectoryStatus));
        CHECK_ALLOC(directory_status);

        // Initialize the directory_status array
        for (int j = 0; j < num_input_dirs; j++) {
            directory_status[j].input_dir = j;
            directory_status[j].modification_time = 0;
            directory_status[j].is_newest = false;
        }

        time_t latest_time = 0;

        // First loop: Store information and find the latest modification time
        LIST *temp = current;
        while (temp) {
            int idx = temp->input_directory_index;
            directory_status[idx].modification_time = temp->file_info;

            if (temp->file_info > latest_time) {
                latest_time = temp -> file_info;
                num_old_files++;
            }

            if (temp->file_info != latest_time) {
                change_required = true;
            }

            temp = temp->next;
        }

        // Second loop: Set the is_newest flag
        temp = current;
        while (temp) {
            int idx = temp->input_directory_index;
            if (temp->file_info == latest_time) {
                directory_status[idx].is_newest = true;
            }

            temp = temp->next;
        }

        if (num_old_files > 0) {
            change_required = true;
        }

        taskqueue_enqueue(tasks, change_required, current->filepath, directory_status, num_input_dirs);


        // Free the allocated memory for directory_status array
        free(directory_status);

    }

    free(hashtable);
}

void print_tasks() {
    taskqueue_print(tasks);
}
