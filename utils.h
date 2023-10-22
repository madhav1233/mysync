#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

void remove_trailing_slash(char *path);
void remove_parent_dir(const char* path, char* filepath);
void usage();
bool check_dir(const char *dir_name);

#endif // UTILS_H
