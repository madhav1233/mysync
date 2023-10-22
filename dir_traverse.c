#include "mysync.h"
#include "utils.h"
#include "dir_traverse.h"
#include "cmd_parser.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include "pattern_match.h"


extern Flags flags;
extern HASHTABLE *hashtable;
extern int num_input_dirs;

#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

void traverse_directory(const char *path, const char *recur_path, int directory_index) {
    DIR *dir;
    struct dirent *entry;
    struct stat s;

    // Open the directory
    dir = opendir(path);
    if (dir == NULL) {
        perror("Unable to open directory");
        printf("%s\n", path);
        usage();
        return;
    }

    // Read entries from the directory
    while ((entry = readdir(dir)) != NULL) {
        // Construct full path
        bool o_flag_match = false;
        bool i_flag_match = false;

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);


        // Use stat to get the file attributes
        if (stat(full_path, &s) == 0) {
            // Check if it's a regular file
            if (S_ISREG(s.st_mode)) {
                char *file_path = strdup(full_path);
                remove_parent_dir(recur_path, file_path);

                if(!flags.a_flag && entry->d_name[0] == '.') {
                    continue;
                }

                for (int i = 0; i < flags.num_o_flags; i++) {
                    if (match_regex(flags.o_flag_obj[i].regex, entry->d_name)) {
                        o_flag_match = true;
                        break;
                    }
                }

                for (int i = 0; i < flags.num_i_flags; i++) {
                    if (match_regex(flags.i_flag_obj[i].regex, entry->d_name)) {
                        i_flag_match = true;
                        break;
                    }
                }

                if (flags.o_flag && !o_flag_match) {
                    continue;
                }

                if (flags.i_flag && i_flag_match) {
                    continue;
                }


                if(flags.v_flag) {
                    printf("File: %s\n", file_path);
                }

                hashtable_add(hashtable, file_path, directory_index, s.st_mtime);
                free(file_path);
            } else if (S_ISDIR(s.st_mode) && flags.r_flag) {
                traverse_directory(full_path, recur_path, directory_index);  // Recursive call
            }
        }
    }

    // Close the directory
    closedir(dir);
}
