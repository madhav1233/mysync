#include "utils.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void remove_trailing_slash(char *path) {
    size_t len = strlen(path);
    if (len > 0 && path[len - 1] == '/') {
        path[len - 1] = '\0';
    }
}

void remove_parent_dir(const char *input_dir, char* filepath) { //TODO: INPUT DIRECTORY MIGHT BE SEVERAL SUBDIRS DEEP, IMPLEMENT FOR THIS
    int input_dir_len = strlen(input_dir);
    if (strncmp(filepath, input_dir, input_dir_len) == 0) {
        // Shift the remaining part of the filepath to the beginning
        memmove(filepath, filepath + input_dir_len, strlen(filepath) - input_dir_len + 1);
    }
}


bool is_regular_file(const char *path) {
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool check_dir(const char *dir_name) {
    DIR* dir = opendir(dir_name);
    if (dir) {
        closedir(dir);
        return true;
    } else {
        fprintf(stderr, "Error: Cannot read directory '%s'.\n", dir_name);
        return false;
    }
}

void usage() {
    printf("Usage: mysync [options] dir1 dir2 [dir3 ....]\n");
    printf("Options:\n");
    printf("  -a \t\t\tsynchronise all files\n");
    printf("  -i pattern \t\tignore files matching the pattern\n");
    printf("  -n \t\t\tdry run\n");
    printf("  -o pattern \t\tonly synchronise files matching the pattern\n");
    printf("  -p \t\t\tpreserve modification times and protection modes\n");
    printf("  -r \t\t\trecursively synchronise directories\n");
    printf("  -v \t\t\tbe verbose and report actions\n");
}
