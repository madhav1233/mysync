#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hashtable.h"
#include "utils.h"



//  --------------------------------------------------------------------

//  FUNCTION hash_string() ACCEPTS A STRING PARAMETER,
//  AND RETURNS AN UNSIGNED 32-BIT INTEGER AS ITS RESULT
//
//  see:  https://en.cppreference.com/w/c/types/integer

uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash;
}

//  ALLOCATE AND INITIALISE SPACE FOR A NEW HASHTABLE (AN ARRAY OF LISTS)
HASHTABLE *hashtable_new(void)
{
    HASHTABLE   *new = calloc(HASHTABLE_SIZE, sizeof(LIST *));

    CHECK_ALLOC(new);
    return new;
}

//  ADD A NEW STRING TO A GIVEN HASHTABLE
void hashtable_add(HASHTABLE *hashtable, char *filepath, int directory_index, time_t file_info) {
    uint32_t h   = hash_string(filepath) % HASHTABLE_SIZE;    // choose list

    hashtable[h] = list_add(hashtable[h], filepath, directory_index, file_info);
}

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
bool hashtable_find(HASHTABLE *hashtable, char *string, int directory_index) {
    uint32_t h	= hash_string(string) % HASHTABLE_SIZE;     // choose list

    return list_find(hashtable[h], directory_index);
}


void hashtable_print(HASHTABLE *hashtable) {
    for (int i = 0; i < HASHTABLE_SIZE; i++) {
        LIST *entry = hashtable[i];
        while (entry != NULL) {
            char *time_str = ctime(&entry->file_info);
            time_str[strcspn(time_str, "\n")] = '\t';  // Remove the newline character
            printf("%s", time_str);
            printf("%d\t", entry->input_directory_index);
            printf("%s\n", entry->filepath);
            // Handle directory list or other fields if needed
            entry = entry->next;
        }
    }
}
