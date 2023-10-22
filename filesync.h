// filesync.h

#ifndef FILESYNC_H
#define FILESYNC_H

#include <stdbool.h>
#include <time.h>

// Function prototypes
int copy_file(const char *source_path, const char *destination_path);
void copy_check(void);

#endif // FILESYNC_H

