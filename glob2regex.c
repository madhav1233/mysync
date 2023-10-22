#include "glob2regex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//  CONVERT A FILENAME PATTERN (a glob) TO A REGULAR EXPRESSION.
//  FILENAME PATTERNS MAY NOT INCLUDE DIRECTORY SEPARATORS.
//	ON SUCCESS - A REGULAR EXPRESSION WILL BE RETURNED IN DYNAMICALLY
//		     ALLOCATED MEMORY.
//	ON FAILURE - A NULL POINTER WILL BE RETURNED.

char *glob2regex(char *glob)
{
    char *re = NULL;

    if(glob != NULL) {
	re	= calloc(strlen(glob)*2 + 4, sizeof(char));
        if(re == NULL) {
	    return NULL;
	}

	char *r	= re;

	*r++	= '^';
	while(*glob != '\0')
	    switch (*glob) {
		case '.' :
		case '\\':
		case '$' : *r++ = '\\'; *r++ = *glob++;	break;
		case '*' : *r++ = '.';  *r++ = *glob++;	break;
		case '?' : *r++ = '.'; glob++;		break;
		case '/' : free(re);
			   re	= NULL;
			   break;
		default  : *r++ = *glob++;
			   break;
	    }
	if(re) {
	    *r++	= '$';
	    *r		= '\0';
	}
    }
    return re;
}
