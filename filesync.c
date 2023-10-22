#include "mysync.h"
#include "task_queue.h"
#include "cmd_parser.h"
#include "utils.h"
#include <sys/stat.h>
#include <utime.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#define BUFFER_SIZE 4096  // You can adjust this buffer size as needed

#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

extern TaskQueue *tasks;
extern int num_input_dirs;
extern char input_directories[MAX_DIRS][MAX_STRING_LENGTH];
extern Flags flags;

int ensure_directory_exists(char *filepath) {
    char *path_copy = strdup(filepath);
    char *dir = dirname(path_copy);

    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        // Directory doesn't exist, try to create parent directories recursively
        ensure_directory_exists(dir);  // Recursive call

        // Now create the current directory
        if (mkdir(dir, 0600) == -1) {
            perror("Failed to create directory");
            free(path_copy);
            return -1;
        }
    }

    free(path_copy);
    return 0;
}


int copy_file(const char *source_path, char *destination_path) {
    if (ensure_directory_exists(destination_path) != 0) {
        return -1;
    }
    FILE *source, *destination;
    char buffer[BUFFER_SIZE];
    size_t bytes;
    struct stat source_stat;
    struct utimbuf new_times;

    // Get the modification time of the source file
    if (stat(source_path, &source_stat) != 0) {
        perror("Failed to get source file stats");
        return -1;
    }

    // Open the source file for reading
    source = fopen(source_path, "rb");
    if (!source) {
        perror("Failed to open source file for reading");
        return -1;
    }

    // Open the destination file for writing
    destination = fopen(destination_path, "wb");
    if (!destination) {
        perror("Failed to open destination file for writing");
        fclose(source);
        return -1;
    }

    // Read from the source file and write to the destination file
    while ((bytes = fread(buffer, 1, BUFFER_SIZE, source)) > 0) {
        if (fwrite(buffer, 1, bytes, destination) != bytes) {
            perror("Error writing to destination file");
            fclose(source);
            fclose(destination);
            return -1;
        }
    }

    // Close both files
    fclose(source);
    fclose(destination);

    
    if (flags.p_flag) {
        // Set the modification time of the destination file
        new_times.actime = source_stat.st_atime;  // Access time
        new_times.modtime = source_stat.st_mtime; // Modification time
                                                  
        if (utime(destination_path, &new_times) != 0) {
            perror("Failed to set modification time on destination file");
            return -1;
        }
        // Set the file permissions to match the source file
        if (chmod(destination_path, source_stat.st_mode) != 0) {
            perror("Failed to set protection mode on destination file");
            return -1;
        }

        // Set the file ownership to match the source file
        if (chown(destination_path, source_stat.st_uid, source_stat.st_gid) != 0) {
            perror("Failed to set ownership on destination file");
            return -1;
        }
    }

    else {
        // Set the modification time of the destination file
        new_times.actime = 0600;  // Access time
                                                  
        if (chmod(destination_path, new_times.actime) != 0) {
            perror("Failed to set protection mode on destination file");
            return -1;
        }
    }

    return 0;  // Return 0 on success
}


void copy_check() {
    TaskQueueItem *current = tasks->head;

    while(current) {
        int newest_dir_idx = -1;
        time_t newest_time = 0;
        for (int i = 0; i < num_input_dirs; i++) {
            if (current->directories[i].is_newest) {
                newest_dir_idx = i;
                newest_time = current->directories[i].modification_time;
                break;  // Assuming only one directory can have the newest file
            }
        }

        // If no newest file was found, move to the next item
        if (newest_dir_idx == -1) {
            current = current->next;
            continue;
        }

        // Construct the source path for the newest file
        char source_path[MAX_STRING_LENGTH];
        int required_length_source = strlen(input_directories[newest_dir_idx]) + strlen(current->filepath) + 2;  // +1 for '/' and +1 for '\0'
        if (required_length_source < MAX_STRING_LENGTH) {
            remove_trailing_slash(input_directories[newest_dir_idx]);
            snprintf(source_path, sizeof(source_path), "%s%s", input_directories[newest_dir_idx], current->filepath);
        } else {
            fprintf(stderr, "Error: Source path is too long to fit in buffer.\n");
            continue;  // Skip this iteration
        }

        // Copy the newest file to other directories where the file doesn't exist or is older
        for (int i = 0; i < num_input_dirs; i++) {
            if (i != newest_dir_idx && (current->directories[i].modification_time == 0 || current->directories[i].modification_time < newest_time)) {
                char destination_path[MAX_STRING_LENGTH];
                int required_length = strlen(input_directories[i]) + strlen(current->filepath) + 2;  // +1 for '/' and +1 for '\0'
                if (required_length < MAX_STRING_LENGTH) {
                    remove_trailing_slash(input_directories[i]);
                    snprintf(destination_path, sizeof(destination_path), "%s%s", input_directories[i], current->filepath);
                }

                if (flags.v_flag) {
                    printf("Copying file '%s' to '%s'\n", source_path, destination_path);
                }

                if (flags.n_flag) {
                    continue;  // Skip this iteration
                }
                copy_file(source_path, destination_path);
            }
        }

        // Move to the next item in the task queue
        current = current->next;
    }
}
