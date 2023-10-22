#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "list.h"

//  ON LINUX WE NEED TO PROTOTYPE THE (NON-STANDARD) strdup() FUNCTION 
//  WHY?  https://stackoverflow.com/questions/32944390/what-is-the-rationale-for-not-including-strdup-in-the-c-standard

#if	defined(__linux__)
extern	char	*strdup(char *string);
#endif

//  ---------------------------------------------------------------------

//  'CREATE' A NEW, EMPTY LIST - JUST A NULL POINTER
LIST *list_new(void) {
    return NULL;
}

//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
bool list_find(LIST *list, int wanted) {
    while(list != NULL) {
        if(list->input_directory_index == wanted) {
            return true; }
    list	= list->next; }
    return false;
}

//  ALLOCATE SPACE FOR A NEW LIST ITEM, TESTING THAT ALLOCATION SUCCEEDS
LIST *list_new_item(char *filepath, int directory_index, time_t file_info) {
    LIST *new       = calloc(1, sizeof(LIST) );
    CHECK_ALLOC(new);
    new->filepath  = strdup(filepath);
    new->input_directory_index =  directory_index;
    new->file_info = file_info;
    new->next       =  NULL;
    return new;
}

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
LIST *list_add(LIST *list, char *filepath, int directory_index, time_t file_info) {
    if(list_find(list, directory_index)) {            // only add each item once
        return list;
    }
    else {                                      // add new item to head of list
        LIST *new   = list_new_item(filepath, directory_index, file_info);
        new->next   = list;
        return new;
    }
}

//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
void list_print(LIST *list) {
    if(list != NULL) {
        while(list != NULL) {
	    printf("Directory Index: %d", list->input_directory_index);
	    if(list->next != NULL) {
	        printf(" -> "); }
	    list	= list->next; }
	printf("\n"); }
}
