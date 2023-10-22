#include "mysync.h"
#include "cmd_parser.h"
#include "filesync.h"
#include "utils.h"
#include "dir_traverse.h"
#include "file_ops.h"
#include "filesync.h"
#include <bits/getopt_core.h>

int num_input_dirs = 0;
Flags flags = {false, false, false, false, false, false, false, 0, 0, NULL, NULL};

HASHTABLE *hashtable;
char input_directories[MAX_DIRS][MAX_STRING_LENGTH];

int main(int argc, char *argv[]) {

    if (argc < 3) {
        usage();
        exit(1);
    }

    parseArguments(argc, argv);

    int dir_index = 0;  // Declare dir_index here
    // check if input directories exists
    for (int i = optind; i < argc; i++) {
        if (!check_dir(argv[i])) {
            usage();
            exit(EXIT_FAILURE);
        }

        if (strlen(argv[i]) < MAX_STRING_LENGTH) {
            strcpy(input_directories[dir_index], argv[i]);  // Use dir_index here
            dir_index++;  // Increment dir_index
        } else {
            printf("Source string is too long to copy!\n");
        }
    }

    num_input_dirs = dir_index;

    if (num_input_dirs < 2) {
        fprintf(stderr, "Error: At least two input directories are required.\n");
        usage();
        exit(EXIT_FAILURE);
    }

    // traverse directories
    
    hashtable = hashtable_new();


    for (int i = 0; i < num_input_dirs; i++) {
        traverse_directory(argv[optind + i], argv[optind + i], i);
    }
    
    if (flags.v_flag) {
        printf("Number of input directories: %d\n", num_input_dirs);
        hashtable_print(hashtable);
    }

    // traverse hashtable
    traverse_hashtable(hashtable);

    if (flags.v_flag) {
        print_tasks();
    }

    // execute tasks
    copy_check();


    return 0;
}
